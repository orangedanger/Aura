// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHealth.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/InterAction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//»ñÈ¡VigourÊôÐÔ
	VigourDef.AttributeToCapture = UAuraAttributeSet::GetVigourAttribute();
	VigourDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigourDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigourDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{	
	const FGameplayTagContainer* SoureTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TagertTag= Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SoureTag;
	EvaluateParameters.TargetTags = TagertTag;
	
	float Vigour = 0.f;
	GetCapturedAttributeMagnitude(VigourDef, Spec, EvaluateParameters, Vigour);
	Vigour = FMath::Max(Vigour, 0.f);

	checkf(Spec.GetContext().GetSourceObject(), TEXT("Please AddSourceObject in ApplyEffect Class "));
	
	int32 Level = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	

	const float MaxHealth = 80.f + 10 * Level + 2.5 * Vigour;

	return MaxHealth;
}
