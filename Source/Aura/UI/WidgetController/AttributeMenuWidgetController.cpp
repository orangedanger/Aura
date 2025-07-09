// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeMenuWidgetController.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/AuraDataAsset.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AuraGameplayTags.h"

void UAttributeMenuWidgetController::InitBroadcast()
{
	checkf(AssetInfo, TEXT("检查 AssetInfo 是否加入到 蓝图AttributeMenuWidgetController中"));

	for (const auto& Pair : GetAuraAS()->TagToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}

	AttributePoinitDelegate.Broadcast(GetAuraPS()->GetAttributePoint());
}

void UAttributeMenuWidgetController::BroadcastCallbackToDependences()
{
	for (const auto& Pair : GetAuraAS()->TagToAttribute)
	{
		AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	 
	GetAuraPS()->OnAttributePointChangeDelegate.AddLambda(
		[this](int32 AttributePoint)
		{
			AttributePoinitDelegate.Broadcast(AttributePoint);
		}
	);
}


void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FDataAssetInfo Info = AssetInfo->FindAttributeInfoFromTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);


}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAuraASC())
	{
		GetAuraASC()->UpgradeAttribute(AttributeTag);
	}
}
