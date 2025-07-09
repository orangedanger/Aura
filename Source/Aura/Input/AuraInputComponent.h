// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Aura/Input/AuraInputConfig.h"
#include "AuraInputComponent.generated.h"



/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:

	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HoldFuncType>
	void BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
void UAuraInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& Action: InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.GameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.GameplayTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.GameplayTag);
			}

			if (HoldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HoldFunc, Action.GameplayTag);
			}
		}
	}
}
