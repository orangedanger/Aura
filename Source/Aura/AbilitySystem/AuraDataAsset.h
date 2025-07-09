// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FDataAssetInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TagName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TagComment = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	FDataAssetInfo FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, bool bNotLogFind = false) const;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDataAssetInfo>  AttributeInfomation;
};
