// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraDamageGameplayAbility.h"
#include"AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


void UAuraDamageGameplayAbility::CasusDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (const auto& Damage : Damages)
	{
		const float ScaleDamage = Damage.Value.GetValueAtLevel(GetAbilityLevel());
		//EffectSpec��Tag���˺�
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Damage.Key, ScaleDamage);
	}

	//thisActor��TargetActor����Effect
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));


}
