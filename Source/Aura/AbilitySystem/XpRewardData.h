// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "XpRewardData.generated.h"

USTRUCT(BlueprintType)
struct FXpRewardAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 RequiredXp =0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 ShellPoint = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	int32 AttributePoint = 1;

};

/**
 * 
 */
UCLASS()
class AURA_API UXpRewardData : public UDataAsset
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XpRewardData")
	TArray<FXpRewardAsset> XpRewardDataArray;

	UFUNCTION(BlueprintCallable, Category = "XpRewardData")
	int32 FindLevelAtXp(int32 inXp);


};
