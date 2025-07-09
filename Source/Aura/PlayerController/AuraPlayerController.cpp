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

	//绑定增强输出组件
	check(Context);
	UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsytem)
	{
		Subsytem->AddMappingContext(Context, 0);
	}

	//鼠标样式
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//鼠标控制和鼠标的显示
	FInputModeGameAndUI InputDate;
	InputDate.SetHideCursorDuringCapture(false);
	InputDate.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputDate);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//获取增强输入组件
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	//增加绑定
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
	* 检测的多种情况
	*	- 1.LastInterface = null , ThisInterface = null
	*			什么都不做
	*
	*	- 2.LastInterface = null , ThisInterface is value
	*			ThisInterface显示高亮HighLight
	*
	*	- 3.LastInterface is value , ThisInterface is null
	*			LastInterface取消显示高亮 UnHighLight
	*
	*	- 4.LastInterface is value , ThisInterface is value , but LastInterface !=ThisInterface
	*			取消LastInterface高亮，显示ThisInterface高亮
	*
	*	-5.LastInterface is value and ThisInterface == LastInterface
	*			什么都不做
	**/
	if (LastInterface == nullptr)
	{
		if (ThisInterface == nullptr)
		{
			//1. 什么都不做
		}
		else
		{
			//2.ThisInterface显示高亮HighLight
			ThisInterface->HighLightAction();
		}
	}
	else
	{
		if (ThisInterface == nullptr)
		{
			//3.LastInterface取消显示高亮 UnHighLight
			LastInterface->UnHighLightAction();
		}
		else
		{
			if (ThisInterface != LastInterface)
			{
				//4.取消LastInterface高亮，显示ThisInterface高亮
				LastInterface->UnHighLightAction();
				ThisInterface->HighLightAction();
			}
			else
			{
				//5.什么都不做
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
//不是左键的功能激活其对应 的Ability
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().Ability_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	//左键的功能

	if (bHitTarget)
	{
		//左键触碰其他角色的功能
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	else
	{
		APawn* ControlledPawn = GetPawn();

		//通过按住的时间长短决定是否触发
		if (ControlledPawn && FollowTime <= ShortPressTime)
		{
			//进行寻路
			if (UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CacheHitVector))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : Path->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				//更新鼠标点击点防止鼠标点到不可到达地人物不停止移动
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

	//不是左键的功能激活其对应 的Ability
	if (!GameplayTag.MatchesTagExact(FAuraGameplayTags::Get().Ability_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	//左键的功能

	if (bHitTarget)
	{
		//左键触碰其他角色的功能
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(GameplayTag);
		}
		return;
	}
	else
	{
		//左键触碰的地面等
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
	//通过矩阵获取X,Y方向大小
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