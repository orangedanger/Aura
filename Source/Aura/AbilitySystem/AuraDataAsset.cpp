// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraDataAsset.h"

FDataAssetInfo UAuraDataAsset::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag, bool bNotLogFind) const
{

	for (const FDataAssetInfo& DataInfo : AttributeInfomation)
	{
		if (DataInfo.GameplayTag.MatchesTagExact(AttributeTag))
		{
			return DataInfo;
		}
	}

	if (bNotLogFind)
	{
		UE_LOG(LogTemp, Error, TEXT("Not FInd Attribute [%s] In AuraDataAsset [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FDataAssetInfo();
}
