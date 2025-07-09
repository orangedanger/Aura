// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraInputConfig;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UFloatingTextWidgetComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();

	UFUNCTION(Client, Reliable)
	void ClientShowFloatingDamageText(ACharacter* TargetActor, float DamageFloat, bool bCritical, bool bEvade);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& InputValue);

	//����Ƿ����ָ������
	void CursorEnemy();

	//Ability������������
	void AbilityInputTagPressed(FGameplayTag GameplayTag);
	void AbilityInputTagReleased(FGameplayTag GameplayTag);
	void AbilityInputTagHeld(FGameplayTag GameplayTag);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> Context;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	IEnemyInterface* LastInterface;
	IEnemyInterface* ThisInterface;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraInputConfig> InputConfig;

	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FHitResult HitResult;

	float FollowTime = 0.f;
	UPROPERTY(EditAnywhere)
	float ShortPressTime = 0.5f;

	FVector CacheHitVector = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float StopRuningRadius = 50.f;

	//�Ƿ��������������ɫ�����ǵص㣬������������ɫΪtrue
	bool bHitTarget = false;
	bool bAutoRuning = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr <USplineComponent> Spline;

	void AutoRunning();

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UFloatingTextWidgetComponent> DamageWidgetClass;
};
