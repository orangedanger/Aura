// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//别名模板
template<class T>
using TAttributePtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FGameplayEffectParams
{
	GENERATED_BODY()

	FGameplayEffectParams() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;

	void ShowFloatingText(FGameplayEffectParams& EffectParams, const float LocalIncomingDamage, const bool bCriticalHit, const bool bEvade);

	
	TMap<FGameplayTag, FGameplayAttribute(*)()> TagToAttribute;

public:

	/**
	*	Vital Attribute 
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;


	/**
	*	Primary Attribute
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attribute")
	FGameplayAttributeData Resilience;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigour, Category = "Primary Attribute")
	FGameplayAttributeData Vigour;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attribute")
	FGameplayAttributeData Agility;

	/**
	*	Secondary Attribute
	*/

	//最大生命和魔力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attribute")
	FGameplayAttributeData MaxMana;

	//攻击力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalAttack, Category = "Secondary Attribute")
	FGameplayAttributeData PhysicalAttack;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalAttack, Category = "Secondary Attribute")
	FGameplayAttributeData MagicalAttack;
	//穿透
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData PhysicalPenetration;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData MagicalPenetration;
	//暴击率和暴击加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitBonus, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitBonus;
	//防御
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalDefense, Category = "Secondary Attribute")
	FGameplayAttributeData PhysicalDefense;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalDefense, Category = "Secondary Attribute")
	FGameplayAttributeData MagicalDefense;
	//生命恢复
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	//法术恢复
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	//命中加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HitBonus, Category = "Secondary Attribute")
	FGameplayAttributeData HitBonus;
	//闪避率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Evade, Category = "Secondary Attribute")
	FGameplayAttributeData Evade;
	/**
	* Damage
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData IncomingDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Xp")
	FGameplayAttributeData IncomingXp;

	/**
	*	Vital Attribute
	*/

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth)const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana)const;


	/**
	*	Primary Attribute
	*/
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience)const;
	UFUNCTION()
	void OnRep_Vigour(const FGameplayAttributeData& OldVigour)const;
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility)const;

	/**
	*	Secondary Attribute
	*/
	UFUNCTION()
	void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack)const;
	UFUNCTION()
	void OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack)const;
	UFUNCTION()
	void OnRep_PhysicalPenetration(const FGameplayAttributeData& OldPhysicalPenetration)const;
	UFUNCTION()
	void OnRep_MagicalPenetration(const FGameplayAttributeData& OldMagicalPenetration)const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)const;
	UFUNCTION()
	void OnRep_CriticalHitBonus(const FGameplayAttributeData& OldCriticalHitBonus)const;
	UFUNCTION()
	void OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)const;
	UFUNCTION()
	void OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense)const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)const;

	UFUNCTION()
	void OnRep_HitBonus(const FGameplayAttributeData& OldHitBonus)const;
	UFUNCTION()
	void OnRep_Evade(const FGameplayAttributeData& OldHitBonus)const;


	void SetGameplayEffectPramars(const FGameplayEffectModCallbackData& Data, FGameplayEffectParams& Props);

private:

	void SendXpEvent(FGameplayEffectParams& Props);

public:
	/*
	* Can Get Attribute variables Get,Set,Init Functions
	**/
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigour);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility);

	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalAttack);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalDefense);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalPenetration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MagicalAttack);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MagicalDefense);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MagicalPenetration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitBonus);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HitBonus);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Evade);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXp);

	private:
		bool bTopOffHealth = false;
		bool bTopOffMana = false;
};
