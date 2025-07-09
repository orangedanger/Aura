// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfo.h"
#include "Aura/LogCategory.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByAbilityTag(const FGameplayTag& AbilityTag, bool bLogNotFind)
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilityInfomation)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFind)
	{
		UE_LOG(AuraLog, Warning, TEXT("Not Find AbilityInfo Has AbilityTag [%s]"), *AbilityTag.ToString());
	}


	return FAuraAbilityInfo();
}
