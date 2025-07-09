// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToXp(int32 Xp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 FindLevelForXp(int32 Xp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerXp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUp(int32 UpValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePointReward(int32 Level);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetShellPointReward(int32 Level);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToAttributePoint(int32 AttributePoint);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToShellPoint(int32 ShellPoint);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetShellPoint();
};
