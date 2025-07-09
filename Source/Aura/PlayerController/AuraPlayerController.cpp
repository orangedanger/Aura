// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Aura/Input/AuraInputComponent.h"
#include "Aura/Input/AuraInputConfig.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "../InterAction/EnemyInterface.h"
#include "Aura/AuraGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

#include "Aura/UI/WidgetComponent/FloatingTextWidgetComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = false;
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//����ǿ������
	check(Context);
	UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsytem)
	{
		Subsytem->AddMappingContext(Context, 0);
	}

	//�����ʽ
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//�����ƺ�������ʾ
	FInputModeGameAndUI InputDate;
	InputDate.SetHideCursorDuringCapture(false);
	InputDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputDate);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//��ȡ��ǿ�������
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//���Ӱ�
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	AuraInputComponent->BindAbilityAction(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CursorEnemy();
	AutoRunning();
}

void AAuraPlayerController::AutoRunning()
{
	if (!bAutoRuning) return;

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		const FVector NextPoint = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector NextDir = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(NextDir);
		const float DistanceToDestination = (CacheHitVector - NextPoint).Length();
		if (DistanceToDestination <= StopRuningRadius)
		{
			bAutoRuning = false;
		}
	}
}

void AAuraPlayerController::CursorEnemy()
{
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	LastInterface = ThisInterface;
	ThisInterface = Cast<IEnemyInterface>(HitResult.GetActor());

	/**
	* ���Ķ������
	*	- 1.LastInterface = null , ThisInterface = null
	*			ʲô������
	*
	*	- 2.LastInterface = null , ThisInterface is value
	*			ThisInterface��ʾ����HighLight
	*
	*	- 3.LastInterface is value , ThisInterface is null
	*			LastInterfaceȡ����ʾ���� UnHighLight
	*
	*	- 4.LastInterface is value , ThisInterface is value , but LastInterface !=ThisInterface
	*			ȡ��LastInterface��������ʾThisInterface����
	*
	*	-5.LastInterface is value and ThisInterface == LastInterface
	*			ʲô������
	**/
	if (LastInterface == nullptr)
	{
		if (ThisInterface == nullptr)
		{
			//1. ʲô������
		}
		else
		{
			//2.ThisInterface��ʾ����HighLight
			ThisInterface->HighLightAction();
		}
	}
	else
	{
		if (ThisInterface == nullptr)
		{
			//3.LastInterfaceȡ����ʾ���� UnHighLight
			LastInterface->UnHighLightAction();
		}
		else
		{
			if (ThisInterface != LastInterface)
			{
				//4.ȡ��LastInterface��������ʾThisInterface����
				LastInterface->UnHighLightAction();
				ThisInterface->HighLightAction();
			}
			else
			{
				//5.ʲô������
			}
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag GameplayTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, *(FString("Pressed ") + GameplayTag.ToString()),false);

	bHitTarget = ThisInterface ? true : false;
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag GameplayTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue,*(FString("Released ") + GameplayTag.ToString()));
//��������Ĺ��ܼ������Ӧ ��Ability
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().Ability_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	//����Ĺ���

	if (bHitTarget)
	{
		//�������������ɫ�Ĺ���
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	else
	{
		APawn* ControlledPawn = GetPawn();

		//ͨ����ס��ʱ�䳤�̾����Ƿ񴥷�
		if (ControlledPawn && FollowTime <= ShortPressTime)
		{
			//����Ѱ·
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheHitVector))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : Path->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				//������������ֹ���㵽���ɵ�������ﲻֹͣ�ƶ�
				if (!Path->PathPoints.IsEmpty())
				{
					CacheHitVector = Path->PathPoints[Path->PathPoints.Num() - 1];
				}
				bAutoRuning = true;
			}
		}
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Yellow, *(FString("Hold ") + GameplayTag.ToString()));

	//��������Ĺ��ܼ������Ӧ ��Ability
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().Ability_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	//����Ĺ���

	if (bHitTarget)
	{
		//�������������ɫ�Ĺ���
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	else
	{
		//��������ĵ����
		bAutoRuning = false;
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (HitResult.bBlockingHit)
		{
			CacheHitVector = HitResult.ImpactPoint;
			if (APawn* ControlledPawn = GetPawn())
			{
				const FVector ForwordDirection = (CacheHitVector - ControlledPawn->GetActorLocation()).GetSafeNormal();
				ControlledPawn->AddMovementInput(ForwordDirection);
			}
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AbilitySystemComponent)
	{
		AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}

void AAuraPlayerController::Move(const FInputActionValue& InputValue)
{
	if (bAutoRuning)
	{
		bAutoRuning = false;
	}

	const FVector2D PlayerAxisVector = InputValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	//ͨ�������ȡX,Y�����С
	const FVector ForwordDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	APawn* ControllerPawn = GetPawn<APawn>();
	if (ControllerPawn)
	{
		ControllerPawn->AddMovementInput(ForwordDir, PlayerAxisVector.Y);
		ControllerPawn->AddMovementInput(RightDir, PlayerAxisVector.X);
	}
}

void AAuraPlayerController::ClientShowFloatingDamageText_Implementation(ACharacter* TargetActor, float DamageFloat, bool bCritical, bool bEvade)
{
	if (DamageWidgetClass)
	{
		UFloatingTextWidgetComponent* DamageText = NewObject<UFloatingTextWidgetComponent>(TargetActor, DamageWidgetClass);
		if (DamageText)
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TargetActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			DamageText->SetFloatingText(DamageFloat, bCritical, bEvade);
		}
	}
}