// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellMenuWidgetController.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AbilityInfo.h"
#include "Aura/PlayerState/AuraPlayerState.h"

void USpellMenuWidgetController::InitBroadcast()
{
	BroadcastAbilityInfo();

	SpellPointDelegate.Broadcast(GetAuraPS()->GetShellPoint());
}

void USpellMenuWidgetController::BroadcastCallbackToDependences()
{
	GetAuraASC()->StateChangeDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StateTag)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByAbilityTag(AbilityTag);
			if (Info.AbilityTag.IsValid())
			{
				Info.StateTag = StateTag;
				OnAbilityInfoSingnature.Broadcast(Info);
			}		
		}
	);

	GetAuraPS()->OnShellPointChangeDelegate.AddLambda([this](int32 ShellPoint)
		{
			SpellPointDelegate.Broadcast(ShellPoint);
		}
	);
}
