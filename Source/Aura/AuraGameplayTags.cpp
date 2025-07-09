// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::AuraGameplayTags;

void FAuraGameplayTags::InitGameplayTags()
{
	/*
	* Secondary Attribute
	*/
	AuraGameplayTags.Attribute_Secondary_PhysicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.PhysicalAttack"),
		FString("����������Ŀ����������˺�")
	);
	AuraGameplayTags.Attribute_Secondary_PhysicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.PhysicalDefense"),
		FString("��������������ܵ��������˺�")
	);
	AuraGameplayTags.Attribute_Secondary_PhysicalPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.PhysicalPenetration"),
		FString("����͸��ʹ��ɵ��˺�һ���̶��ϴ�͸Ŀ����������")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalAttack"),
		FString("ħ����������Ŀ�����ħ���˺�")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalDefense"),
		FString("ħ�������������ܵ���ħ���˺�")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalPenetration"),
		FString("ħ����͸��ʹ��ɵ��˺�һ���̶��ϴ�͸Ŀ���ħ������")
	);
	AuraGameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"),
		FString("�����ָ����ָ������������ٶ� ")
	);
	AuraGameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"),
		FString("ħ���ָ����ָ�����ħ�����ٶ� ")
	);
	AuraGameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitChance"),
		FString("�����ʣ���Ŀ������˺����������ĸ��� ")
	);
	AuraGameplayTags.Attribute_Secondary_CriticalHitBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitBonus"),
		FString("�����ӳɣ���Ŀ������˺���������ʱ�ܵ����˺��ӳ� ")
	);
	AuraGameplayTags.Attribute_Secondary_HitBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HitBonus"),
		FString("���мӳɣ��������Ŀ��ĸ��� ")
	);
	AuraGameplayTags.Attribute_Secondary_Evade = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Secondary.Evade"),
		FString("���ܣ����ܹ�������ֵ")
	);
	AuraGameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"),
		FString("���ħ������������ħ��ֵ")
	);
	AuraGameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"),
		FString("���������������������ֵ")
	);
	/*
	* Primary Attribute
	*/
	AuraGameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Strength"),
		FString("���������������֮һӰ�츱���Ե�ֵ")
	);

	AuraGameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Intelligence"),
		FString("���������������֮һӰ�츱���Ե�ֵ")
	);
	AuraGameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Resilience"),
		FString("���ԣ����������֮һӰ�츱���Ե�ֵ")
	);
	AuraGameplayTags.Attribute_Primary_Vigour = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Vigour"),
		FString("���������������֮һӰ�츱���Ե�ֵ")
	);
	AuraGameplayTags.Attribute_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Agility"),
		FString("���ݣ����������֮һӰ�츱���Ե�ֵ")
	);

	AuraGameplayTags.Attribute_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Agility"),
		FString("���ݣ����������֮һӰ�츱���Ե�ֵ")
	);

	/*
	* Meta
	*/

	AuraGameplayTags.Attribute_Meta_IncomingXp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Meta.IncomingXp"),
		FString("��ʱ����ֵ")
	);

	/*
	* Input
	*/

	AuraGameplayTags.Ability_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.LMB"),
		FString("Input Tag For Left Mouse Button")
	);
	AuraGameplayTags.Ability_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.RMB"),
		FString("Input Tag For Right Mouse Button")
	);
	AuraGameplayTags.Ability_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.1"),
		FString("Input Tag For 1")
	);
	AuraGameplayTags.Ability_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.2"),
		FString("Input Tag For 2")
	);
	AuraGameplayTags.Ability_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.3"),
		FString("Input Tag For 3")
	);
	AuraGameplayTags.Ability_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Ability.4"),
		FString("Input Tag For 4")
	);

	/*
	* DamageType
	*/
	AuraGameplayTags.Physical_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Damage.Physical"),
		FString("Physical Damage")
	);
	AuraGameplayTags.Lightning_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Damage.Lightning"),
		FString("Lightning Damage")
	);
	AuraGameplayTags.Arcane_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Damage.Arcane"),
		FString("Arcane Damage")
	);
	AuraGameplayTags.Fire_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Damage.Fire"),
		FString("Fire Damage")
	);

	AuraGameplayTags.DamageTypes.Add(AuraGameplayTags.Physical_Damage);
	AuraGameplayTags.DamageTypes.Add(AuraGameplayTags.Fire_Damage);
	AuraGameplayTags.DamageTypes.Add(AuraGameplayTags.Lightning_Damage);
	AuraGameplayTags.DamageTypes.Add(AuraGameplayTags.Arcane_Damage);

	/*
	* Effect
	*/
	AuraGameplayTags.Combat_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Combat.HitReact"),
		FString("Combat HitReact Tag")
	);

	/*
	* Abilities
	*/
	AuraGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Attack"),
		FString("��ͨ��������")
	);
	AuraGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Summon"),
		FString("�ٻ�����")
	);
	AuraGameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Fire.FireBolt"),
		FString("������� ")
	);

	AuraGameplayTags.Abilities_Lightning_ThunderStroke = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Lightning.ThunderStroke"),
		FString("�׻�      ")
	);

	/*
	* Spell States
	*/
	AuraGameplayTags.Spell_State_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		
		FName("Spell.State.Unlocked"),
		FString("���ܽ���״̬ ")
	);

	AuraGameplayTags.Spell_State_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Locked"),
		FString("��������״̬ ")
	);

	AuraGameplayTags.Spell_State_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Equipped"),
		FString("����װ��״̬ ")
	);

	AuraGameplayTags.Spell_State_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Eligible"),
		FString("���ܿ��Ի��Ѽ��ܵ㿪��״̬ ")
	);


	/*
	* Spell Type
	*/
	AuraGameplayTags.Spell_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.Offensive"),
		FString("�������� ")
	);

	AuraGameplayTags.Spell_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.Passive"),
		FString("�������� ")
	);

	AuraGameplayTags.Spell_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.None"),
		FString("�����   ")
	);



	/*
	* Cooldown
	*/
	AuraGameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Cooldown.Fire.FireBolt"),
		FString("�����������ȴ ")
	);

	/*
	* Montage
	*/
	AuraGameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"),
		FString("��������   ")
	);

	AuraGameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.LeftHand"),
		FString("���ֹ���   ")
	);

	AuraGameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.RightHand"),
		FString("���ֹ���   ")
	);

	AuraGameplayTags.Montage_Attack_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.Tail"),
		FString("β�͹���   ")
	);

	AuraGameplayTags.Montage_Summon_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Summon.Weapon"),
		FString("��������   ")
	);
}