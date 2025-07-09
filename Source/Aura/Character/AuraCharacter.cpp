// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Aura/UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	//开始后可以旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//可以在哪个方向旋转
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	//目的是是目标对齐到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	//通过Controller控制方向关闭
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);

	LevelupEffect = CreateDefaultSubobject<UNiagaraComponent>("Levelup Effect");
	LevelupEffect->SetupAttachment(GetRootComponent());
	LevelupEffect->bAutoActivate = false;

}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//在服务器端进行初始化
	InitialAbilityActor();
	GrantGameplayAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//在客户器端进行初始化
	InitialAbilityActor();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraState, TEXT("Please Set BP_AuraPlayerState in BP_AuraGameMode First"));
	return AuraState->GetPlayerLevel();
}

void AAuraCharacter::AddToXp_Implementation(int32 Xp)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		AuraState->AddXp(Xp);
	}
}

int32 AAuraCharacter::FindLevelForXp_Implementation(int32 Xp)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->XpData.Get()->FindLevelAtXp(Xp);
	}
	return 0;
}

int32 AAuraCharacter::GetPlayerXp_Implementation()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->GetPlayerXp();
	}
	return 0;
}

void AAuraCharacter::LevelUp_Implementation(int32 UpValue)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		MulticastLevelupEffect();
		AuraState->AddLevel(UpValue);
	}

}

int32 AAuraCharacter::GetAttributePointReward_Implementation(int32 Level)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->XpData.Get()->XpRewardDataArray[Level].AttributePoint;
	}
	return 0;
}

int32 AAuraCharacter::GetShellPointReward_Implementation(int32 Level)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->XpData.Get()->XpRewardDataArray[Level].ShellPoint;
	}
	return 0;
}

void AAuraCharacter::AddToAttributePoint_Implementation(int32 AttributePoint)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		AuraState->AddAttributePoint(AttributePoint);
	}
}

void AAuraCharacter::AddToShellPoint_Implementation(int32 ShellPoint)
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		AuraState->AddShellPoint(ShellPoint);

		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AuraState->GetAbilitySystemComponent());
		if (AuraASC)
		{
			AuraASC->UpdataAbilitiesState(AuraState->GetPlayerLevel());
		}
	}
}

int32 AAuraCharacter::GetAttributePoint_Implementation()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->GetAttributePoint();
	}
	return 0;
}

int32 AAuraCharacter::GetShellPoint_Implementation()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	if (AuraState)
	{
		return AuraState->GetShellPoint();
	}
	return 0;
}

void AAuraCharacter::MulticastLevelupEffect_Implementation()
{
	const  FVector EffectLocation = LevelupEffect->GetComponentLocation();
	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	const FRotator Rotation = (CameraLocation - EffectLocation).Rotation();
	LevelupEffect->SetWorldRotation(Rotation);
	LevelupEffect->Activate(true);
}


void AAuraCharacter::InitialAbilityActor()
{
	AAuraPlayerState* AuraState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraState,TEXT("Please Set BP_AuraPlayerState in BP_AuraGameMode First"));
	AuraState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraState, this);
	AbilitySystemComponement = AuraState->GetAbilitySystemComponent();

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponement)->AbilityActorInfoSet();

	AttributeSet = AuraState->GetAttributSet();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AuraHUD->InitOverlap(PlayerController, AuraState, AbilitySystemComponement, AttributeSet);
		}
	}
	InitialAttribute();
}

 