// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecCalcsDamage.h"
#include"AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitBonus);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Evade);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HitBonus);

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicalPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicalAttack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalAttack, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Evade, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, HitBonus, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalDefense, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatic;
	return DStatic;
}

//进行属性的捕获
UExecCalcsDamage::UExecCalcsDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().EvadeDef);
	RelevantAttributesToCapture.Add(DamageStatics().HitBonusDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalDefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalAttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalAttackDef);
}

void UExecCalcsDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//基础值的设定
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* SoureASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.TargetTags = TargetTag;
	EvaluateParameters.SourceTags = SourceTag;

	if (TargetASC == SoureASC)
	{
		return;
	}

	//伤害
	float Damage = 0.f;

	//命中值
	float HitBonus = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HitBonusDef, EvaluateParameters, HitBonus);

	//闪避
	float Evade = 0.f;
	//获得属性值
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EvadeDef, EvaluateParameters, Evade);

	//传递闪避是否成功
	const bool bEvade = FMath::RandRange(0, 100) < FMath::Max(0, ((Evade - HitBonus) / (Evade + 25)) * 100);
	UAuraAbilitySystemLibrary::SetIsEvade(EffectContextHandle, bEvade);
	if (bEvade)
	{
		Damage = 0.1f;
	}
	else
	{
		for (FGameplayTag DamageTag : FAuraGameplayTags::Get().DamageTypes)
		{
			float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTag,false);

			//魔法防御
			float MagicalDefense = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalDefenseDef, EvaluateParameters, MagicalDefense);
			MagicalDefense = FMath::Max<float>(MagicalDefense, 0);

			//魔法穿透
			float MagicalPenetration = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalPenetrationDef, EvaluateParameters, MagicalPenetration);
			MagicalPenetration = FMath::Max<float>(MagicalPenetration, 0);

			//物理防御
			float PhysicalDefense = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalDefenseDef, EvaluateParameters, PhysicalDefense);
			PhysicalDefense = FMath::Max<float>(PhysicalDefense, 0);

			//物理穿透
			float PhysicalPenetration = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalPenetrationDef, EvaluateParameters, PhysicalPenetration);
			PhysicalPenetration = FMath::Max<float>(PhysicalPenetration, 0);

			//伤害减免(（护甲-穿透）/（护甲+100）)
			const float MagicalDamageReduce = (MagicalDefense - MagicalPenetration) / (MagicalDefense + 100);
			const float PhysicalDamageReduce = (PhysicalDefense - PhysicalPenetration) / (PhysicalDefense + 100);

			if (DamageTag == FAuraGameplayTags::Get().Physical_Damage)
			{
				//物理伤害
				float PhysicalAttack = 0.f;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().PhysicalAttackDef, EvaluateParameters, PhysicalAttack);
				DamageTypeValue = DamageTypeValue * (1 - PhysicalDamageReduce) * (1 + PhysicalAttack / 40.f);
			}
			else
			{
				//魔法伤害
				float MagicalAttack = 0.f;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicalAttackDef, EvaluateParameters, MagicalAttack);

				DamageTypeValue = DamageTypeValue * (1 - MagicalDamageReduce)* (1 + MagicalAttack / 40.f);
			}
			//伤害汇总
			Damage += DamageTypeValue;
		}

		//暴击概率
		float CriticalHitChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, CriticalHitChance);
		CriticalHitChance = FMath::Max<float>(CriticalHitChance, 0);

		//暴击加成
		float CriticalHitBonus = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitBonusDef, EvaluateParameters, CriticalHitBonus);
		CriticalHitBonus = FMath::Max<float>(CriticalHitBonus, 0);

		//传递暴击是否成功
		const bool bCriticalHit = FMath::RandRange(0, 100) < CriticalHitChance;
		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
		if (bCriticalHit)
		{
			Damage = bCriticalHit ? Damage * (1 + CriticalHitBonus) : Damage;
		}

		// 伤害 = （伤害 * (1+暴击加成)）*（1-伤害减免）
		Damage = FMath::FloorToFloat(Damage);
	}

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}