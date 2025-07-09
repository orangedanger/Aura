// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAuraDataAsset;
struct FGameplayTag;
struct FGameplayAttribute;
struct FDataAssetInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoSignature, const FDataAssetInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributePointSignature, int32, AttributePoint);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:

	virtual void InitBroadcast() override;
	virtual void BroadcastCallbackToDependences() override;

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag,const FGameplayAttribute& Attribute)const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributePointSignature AttributePoinitDelegate;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraDataAsset> AssetInfo;
};
