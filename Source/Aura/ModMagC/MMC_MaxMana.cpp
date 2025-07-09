// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxMana.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/InterAction/CombatInterface.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SoureTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TagertTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SoureTag;
	EvaluateParameters.TargetTags = TagertTag;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	checkf(Spec.GetContext().GetSourceObject(), TEXT("Please AddSourceObject in ApplyEffect Class "));
	int32 Level = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}



	return 50.f + 5.f * Level + 2.f * Intelligence;
}
