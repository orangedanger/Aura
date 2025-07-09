// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
/**
 *
 */
struct FAuraGameplayTags
{
public:
	FORCEINLINE static const FAuraGameplayTags& Get() { return AuraGameplayTags; }

	static void InitGameplayTags();

	FGameplayTag Attribute_Secondary_PhysicalAttack;
	FGameplayTag Attribute_Secondary_MagicalAttack;
	FGameplayTag Attribute_Secondary_PhysicalPenetration;
	FGameplayTag Attribute_Secondary_MagicalPenetration;
	FGameplayTag Attribute_Secondary_PhysicalDefense;
	FGameplayTag Attribute_Secondary_MagicalDefense;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitBonus;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_HitBonus;
	FGameplayTag Attribute_Secondary_Evade;

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigour;
	FGameplayTag Attribute_Primary_Agility;

	//Meta

	FGameplayTag Attribute_Meta_IncomingXp;

	//Input
	FGameplayTag Ability_LMB;
	FGameplayTag Ability_RMB;
	FGameplayTag Ability_1;
	FGameplayTag Ability_2;
	FGameplayTag Ability_3;
	FGameplayTag Ability_4;

	//Damage Type
	FGameplayTag Fire_Damage;
	FGameplayTag Lightning_Damage;
	FGameplayTag Arcane_Damage;
	FGameplayTag Physical_Damage;

	TArray<FGameplayTag> DamageTypes;

	//Effect
	FGameplayTag Combat_HitReact;

	//Ability
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_ThunderStroke;


	//Spell States
	FGameplayTag Spell_State_Locked;
	FGameplayTag Spell_State_Eligible;
	FGameplayTag Spell_State_Equipped;
	FGameplayTag Spell_State_Unlocked;

	//Spell Type
	FGameplayTag Spell_Type_Passive;
	FGameplayTag Spell_Type_Offensive;
	FGameplayTag Spell_Type_None;

	//Cooldown
	FGameplayTag Cooldown_Fire_FireBolt;

	//Montage
	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_LeftHand;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_Tail;
	FGameplayTag Montage_Summon_Weapon;

protected:

private:

	static FAuraGameplayTags AuraGameplayTags;
};