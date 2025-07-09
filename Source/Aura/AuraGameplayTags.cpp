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
		FString("物理攻击：对目标造成物理伤害")
	);
	AuraGameplayTags.Attribute_Secondary_PhysicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.PhysicalDefense"),
		FString("物理防御：减少受到的物理伤害")
	);
	AuraGameplayTags.Attribute_Secondary_PhysicalPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.PhysicalPenetration"),
		FString("物理穿透：使造成的伤害一定程度上穿透目标的物理防御")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalAttack"),
		FString("魔法攻击：对目标造成魔法伤害")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalDefense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalDefense"),
		FString("魔法防御：减少受到的魔法伤害")
	);
	AuraGameplayTags.Attribute_Secondary_MagicalPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MagicalPenetration"),
		FString("魔法穿透：使造成的伤害一定程度上穿透目标的魔法防御")
	);
	AuraGameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"),
		FString("生命恢复：恢复自身生命的速度 ")
	);
	AuraGameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"),
		FString("魔力恢复：恢复自身魔力的速度 ")
	);
	AuraGameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitChance"),
		FString("暴击率：对目标造成伤害产生暴击的概率 ")
	);
	AuraGameplayTags.Attribute_Secondary_CriticalHitBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitBonus"),
		FString("暴击加成：对目标造成伤害产生暴击时受到的伤害加成 ")
	);
	AuraGameplayTags.Attribute_Secondary_HitBonus = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.HitBonus"),
		FString("命中加成：提高命中目标的概率 ")
	);
	AuraGameplayTags.Attribute_Secondary_Evade = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Secondary.Evade"),
		FString("闪避：闪避攻击的数值")
	);
	AuraGameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"),
		FString("最大魔力：自身的最大魔力值")
	);
	AuraGameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"),
		FString("最大生命：自身的最大生命值")
	);
	/*
	* Primary Attribute
	*/
	AuraGameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Strength"),
		FString("力量：五大主属性之一影响副属性的值")
	);

	AuraGameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Intelligence"),
		FString("智力：五大主属性之一影响副属性的值")
	);
	AuraGameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Resilience"),
		FString("韧性：五大主属性之一影响副属性的值")
	);
	AuraGameplayTags.Attribute_Primary_Vigour = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Vigour"),
		FString("活力：五大主属性之一影响副属性的值")
	);
	AuraGameplayTags.Attribute_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Agility"),
		FString("敏捷：五大主属性之一影响副属性的值")
	);

	AuraGameplayTags.Attribute_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Attribute.Primary.Agility"),
		FString("敏捷：五大主属性之一影响副属性的值")
	);

	/*
	* Meta
	*/

	AuraGameplayTags.Attribute_Meta_IncomingXp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Meta.IncomingXp"),
		FString("临时经验值")
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
		FString("普通攻击能力")
	);
	AuraGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Summon"),
		FString("召唤能力")
	);
	AuraGameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Fire.FireBolt"),
		FString("发射火球 ")
	);

	AuraGameplayTags.Abilities_Lightning_ThunderStroke = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Abilities.Lightning.ThunderStroke"),
		FString("雷击      ")
	);

	/*
	* Spell States
	*/
	AuraGameplayTags.Spell_State_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		
		FName("Spell.State.Unlocked"),
		FString("技能解锁状态 ")
	);

	AuraGameplayTags.Spell_State_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Locked"),
		FString("技能锁定状态 ")
	);

	AuraGameplayTags.Spell_State_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Equipped"),
		FString("技能装备状态 ")
	);

	AuraGameplayTags.Spell_State_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.State.Eligible"),
		FString("技能可以花费技能点开启状态 ")
	);


	/*
	* Spell Type
	*/
	AuraGameplayTags.Spell_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.Offensive"),
		FString("主动技能 ")
	);

	AuraGameplayTags.Spell_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.Passive"),
		FString("被动技能 ")
	);

	AuraGameplayTags.Spell_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Spell.Type.None"),
		FString("无类别   ")
	);



	/*
	* Cooldown
	*/
	AuraGameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Cooldown.Fire.FireBolt"),
		FString("发射火球技能冷却 ")
	);

	/*
	* Montage
	*/
	AuraGameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"),
		FString("武器攻击   ")
	);

	AuraGameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.LeftHand"),
		FString("左手攻击   ")
	);

	AuraGameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.RightHand"),
		FString("右手攻击   ")
	);

	AuraGameplayTags.Montage_Attack_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Attack.Tail"),
		FString("尾巴攻击   ")
	);

	AuraGameplayTags.Montage_Summon_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(

		FName("Montage.Summon.Weapon"),
		FString("武器能力   ")
	);
}