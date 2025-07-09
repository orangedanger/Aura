// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseDataTargetSingnature, const FGameplayAbilityTargetDataHandle&,DataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
	
public:


	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "CreateTargetData",HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* CreateUnderMouseTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseDataTargetSingnature ValidData;

private:

	virtual void Activate() override;

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle ,FGameplayTag ActivationTag);
};
