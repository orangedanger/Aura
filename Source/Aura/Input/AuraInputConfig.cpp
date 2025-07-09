// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraInputConfig.h"
#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAuraInputActionForTag(const FGameplayTag InputTag) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (InputTag == Action.GameplayTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("There is no find a Tag:[%s] Action In [%s]"), *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}
