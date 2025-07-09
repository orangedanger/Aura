// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "Aura/Aura.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Aura/UI/UserWidget/AuraUserWidget.h"
#include "Aura//AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"BehaviorTree/BehaviorTree.h"
#include"Aura/AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/AbilitySystem/CharacterClassInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/GameMode/AuraGameModeBase.h"


AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilitySystemComponement = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponement->SetIsReplicated(true);
	AbilitySystemComponement->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		if (GameMode->CharacterClassInfo)
		{
			PrimaryEffectClass = GameMode->CharacterClassInfo->GetCharacterDefaultInfo(CharaterClass).PrimaryAttributes;
			VitalEffectClass = GameMode->CharacterClassInfo->VitalAttributes;
			SecondaryEffectClass = GameMode->CharacterClassInfo->SecondaryAttributes;
		}
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;

	InitialAbilityActor();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponement,CharaterClass);
	}

	UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(WidgetComponent->GetUserWidgetObject());
	if (AuraUserWidget)
	{
		AuraUserWidget->SetWidgetController(this);
	}

	UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(GetAttributSet());
	if (AuraAS)
	{
		AbilitySystemComponement->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
				
			}
		);

		AbilitySystemComponement->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponement->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().Combat_HitReact, 
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::HitReactTagChanged);

		OnHealthChange.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	AuraAIController = Cast<AAuraAIController>(NewController);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AuraAIController->RunBehaviorTree(BehaviorTree);
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitReacting"), false);
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bRangedAttack"), CharaterClass !=ECharacterClass::Warrior);
	}
}

void AEnemyCharacter::HighLightAction()
{
	//¿ªÆôäÖÈ¾
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyCharacter::UnHighLightAction()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* CombatActor)
{
	this->CombatTarget = CombatActor;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

int32 AEnemyCharacter::GetPlayerLevel_Implementation()
{
	return Level;
}

void AEnemyCharacter::Death()
{
	SetLifeSpan(5.f);
 	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Death();
}

void AEnemyCharacter::InitialAbilityActor()
{
	AbilitySystemComponement->InitAbilityActorInfo(this, this);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponement)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitialAttribute();
	}
}

void AEnemyCharacter::InitialAttribute() const
{
	UAuraAbilitySystemLibrary::InitDefaultAttribute(this, CharaterClass, Level, GetAbilitySystemComponent());
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0 : BaseSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitReacting"), bHitReacting);
	}
}
