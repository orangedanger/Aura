// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/InterAction/CombatInterface.h"
#include "Aura/Character/EnemyCharacter.h"
#include "Aura/InterAction/PlayerInterface.h"
#include "Aura/AuraAbilityTypes.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/PlayerController/AuraPlayerController.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();

	TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Resilience, GetResilienceAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Vigour, GetVigourAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Primary_Agility, GetAgilityAttribute);

	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_PhysicalAttack, GetPhysicalAttackAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_MagicalAttack, GetMagicalAttackAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_PhysicalDefense, GetPhysicalDefenseAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_MagicalDefense, GetMagicalDefenseAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_PhysicalPenetration, GetPhysicalPenetrationAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_MagicalPenetration, GetMagicalPenetrationAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitBonus, GetCriticalHitBonusAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_HitBonus, GetHitBonusAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_Evade, GetEvadeAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttribute.Add(AuraGameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigour, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Agility, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MagicalDefense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MagicalPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HitBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Evade, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//NewValue is a mutable reference so you are able to clamp the newly applied value as well

	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute()&&bTopOffHealth)
	{
		SetHealth(FMath::Clamp(GetMaxHealth(), 0.f, GetMaxHealth()));
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute()&&bTopOffMana)
	{
		SetMana(FMath::Clamp(GetMaxMana(), 0.f, GetMaxMana()));
		bTopOffMana = false;
	}

}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectParams EffectParams;

	SetGameplayEffectPramars(Data, EffectParams);
	//预处理函数中值修改仍要在应用时进行修改



	//血量变化
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
		//UE_LOG(LogTemp, Warning, TEXT("The Character [%s] health is %f"), *EffectParams.TargetAvatarActor->GetName(), GetHealth());
		
	}

	//魔力变化
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}
	

	//伤害变化
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();

		if (EffectParams.SourceCharacter == EffectParams.TargetCharacter)
		{
			return;
		}

		if (LocalIncomingDamage > 0.f)
		{
			//获取bool属性
			const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
			const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
			const bool bEvade = AuraEffectContext->IsEvade();
			const bool bCriticalHit = AuraEffectContext->IsCritical();

			float NewHealth = GetHealth();
			if (!bEvade)
			{
				NewHealth -= LocalIncomingDamage;
			}

			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			//判断是否死亡
			bool bDeath = NewHealth <= 0.f;

			if (!bDeath)
			{
				if (!bEvade)
				{
					if (EffectParams.TargetCharacter && EffectParams.TargetASC)
					{
						ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectParams.TargetCharacter);
						EffectParams.TargetCharacter->StopAnimMontage(CombatInterface->Execute_GetHitReactMontage(EffectParams.TargetCharacter));
						//播放受击动画
						FGameplayTagContainer TagContainer;
						TagContainer.AddTag(FAuraGameplayTags::Get().Combat_HitReact);
						EffectParams.TargetASC->TryActivateAbilitiesByTag(TagContainer);
					}
				}
			}
			else
			{
				//死亡动画
				ICombatInterface* CombatInterface = CastChecked<ICombatInterface>(EffectParams.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Death();
				}
				SendXpEvent(EffectParams);
			}

			//显示浮动字体
			ShowFloatingText(EffectParams, LocalIncomingDamage, bCriticalHit, bEvade);
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXpAttribute())
	{
		const int32 LocalIncomingXp = GetIncomingXp();
		SetIncomingXp(0);
		//设置PlayerState中的经验值
		if (EffectParams.SourceCharacter->Implements<UPlayerInterface>()&&EffectParams.SourceCharacter->Implements<UCombatInterface>())
		{
			const int32 Level = ICombatInterface::Execute_GetPlayerLevel(EffectParams.SourceCharacter);
			const int32 CurrentXp = IPlayerInterface::Execute_GetPlayerXp(EffectParams.SourceCharacter)+ LocalIncomingXp;

			const int32 CurrentLevel = IPlayerInterface::Execute_FindLevelForXp(EffectParams.SourceCharacter, CurrentXp);

			int32 UpLevel = CurrentLevel - Level;
			if (UpLevel > 0)
			{

				int32 AttributePointsReward = 0;
				int32 ShellPointsReward = 0;

				for (int32 i = 0; i < UpLevel; i++)
				{
					AttributePointsReward += IPlayerInterface::Execute_GetAttributePointReward(EffectParams.SourceCharacter, Level + i);
					ShellPointsReward += IPlayerInterface::Execute_GetShellPointReward(EffectParams.SourceCharacter, Level + i);
				}
				
				IPlayerInterface::Execute_LevelUp(EffectParams.SourceCharacter, UpLevel);

				IPlayerInterface::Execute_AddToAttributePoint(EffectParams.SourceCharacter, AttributePointsReward);
				IPlayerInterface::Execute_AddToShellPoint(EffectParams.SourceCharacter, ShellPointsReward);

				bTopOffHealth = true;
				bTopOffMana = true;
			}
			IPlayerInterface::Execute_AddToXp(EffectParams.SourceCharacter, LocalIncomingXp);
		}
	}
}

void UAuraAttributeSet::SendXpEvent(FGameplayEffectParams& Props)
{
	// 当敌人死亡时触发的事件，获取经验值并发送给玩家
	// Target 是敌人，从敌人中获取经验值
	if (Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		// 获取敌人的等级
		const float TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetAvatarActor);
		// 获取敌人的角色类型
		const ECharacterClass TargetCharacterClass = ICombatInterface::Execute_GetCharacterClass(Props.SourceCharacter);

		// 获取游戏标签
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		// 设置事件标签为获取经验值
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXp;
		// 计算并  设置事件的经验值大小
		Payload.EventMagnitude = UAuraAbilitySystemLibrary::GetRewardXpForClassAndLevel(Props.TargetAvatarActor, TargetLevel, TargetCharacterClass);

		// 发送游戏事件给玩家，传递经验值
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, Payload.EventTag, Payload);
	}
}


void UAuraAttributeSet::ShowFloatingText(FGameplayEffectParams& EffectParams, const float LocalIncomingDamage, const bool bCriticalHit, const bool bEvade)
{
	if (IsValid(EffectParams.SourceController) || IsValid(EffectParams.TargetController))
	{
		AAuraPlayerController* SourcePC = Cast<AAuraPlayerController>(EffectParams.SourceController);
		if (SourcePC)
		{
			SourcePC->ClientShowFloatingDamageText(EffectParams.TargetCharacter, LocalIncomingDamage, bCriticalHit, bEvade);
			return;
		}

		AAuraPlayerController* TargetPC = Cast<AAuraPlayerController>(EffectParams.TargetController);
		if (TargetPC)
		{
			TargetPC->ClientShowFloatingDamageText(EffectParams.TargetCharacter, LocalIncomingDamage, bCriticalHit, bEvade);
		}
	}
}

void UAuraAttributeSet::SetGameplayEffectPramars(const FGameplayEffectModCallbackData& Data, FGameplayEffectParams& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (Props.SourceASC && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);

		if (Props.TargetController == nullptr && Props.TargetAvatarActor)
		{
			if (APawn* Pawn = Cast<APawn>(Props.TargetAvatarActor))
			{
				Props.TargetController = Pawn->GetController();
			}
		}
		if (Props.TargetController)
		{
			Props.TargetCharacter = Props.TargetController->GetCharacter();
		}
	}
}

//GAMEPLAYATTRIBUTE_REPNOTIFY这个宏用于RepNotify函数中，在客户端中修改属性

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigour(const FGameplayAttributeData& OldVigour)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigour, OldVigour);
}

void UAuraAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Agility, OldAgility);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
	MaxHealth.SetBaseValue(MaxHealth.GetCurrentValue());
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UAuraAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UAuraAttributeSet::OnRep_PhysicalPenetration(const FGameplayAttributeData& OldPhysicalPenetration)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalPenetration, OldPhysicalPenetration);
}

void UAuraAttributeSet::OnRep_MagicalPenetration(const FGameplayAttributeData& OldMagicalPenetration)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MagicalPenetration, OldMagicalPenetration);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitBonus(const FGameplayAttributeData& OldCriticalHitBonus)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitBonus, OldCriticalHitBonus);
}

void UAuraAttributeSet::OnRep_PhysicalDefense(const FGameplayAttributeData& OldPhysicalDefense)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalDefense, OldPhysicalDefense);
}

void UAuraAttributeSet::OnRep_MagicalDefense(const FGameplayAttributeData& OldMagicalDefense)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MagicalDefense, OldMagicalDefense);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_HitBonus(const FGameplayAttributeData& OldHitBonus)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HitBonus, OldHitBonus);
}

void UAuraAttributeSet::OnRep_Evade(const FGameplayAttributeData& OldHitBonus)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Evade, OldHitBonus);
}