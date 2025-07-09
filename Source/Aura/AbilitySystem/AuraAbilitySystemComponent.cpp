// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/InterAction/PlayerInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/LogCategory.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/AbilityInfo.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientGameplayEffectAppliedDelegateToSelf);

}

void UAuraAbilitySystemComponent::ClientGameplayEffectAppliedDelegateToSelf_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle)
{
	FGameplayTagContainer Container;

	GameSpec.GetAllAssetTags(Container);

	AssertTag.Broadcast(Container);
}

void UAuraAbilitySystemComponent::GrantGameplayAbilities(const TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClassOf : GameplayAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf, 1);

		if (const UAuraGameplayAbility* GameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//进行标签的绑定,自定义的Tag加入到Ability的动态标签中
			AbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->InputGameplayTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Spell_State_Eligible);
			//将Ability赋予给相应的角色
			GiveAbility(AbilitySpec);
		}
	}
	//初始化UI Ability显示
	bStartAbilityDeleagte = true;
	AbilityDeleagte.Broadcast();
}

void UAuraAbilitySystemComponent::GrantGameplayPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClassOf : GameplayAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf, 1);

		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

 

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(FForEachAbility Delegate)
{
	// 使用作用域锁（Scoped Lock）锁定当前 Ability 列表，确保遍历过程中列表不会被修改。
	// 这是为了避免在迭代过程中因其他线程或逻辑修改 Ability 列表导致的数据竞争或不一致性。
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	// 遍历所有可激活的 Ability（通过 GetActivatableAbilities 获取列表）
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 执行委托，将当前 AbilitySpec 作为参数传递。
		// ExecuteIfBound 返回 `false` 表示委托未绑定或执行失败。
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			// 记录错误日志：输出当前函数名（__FUNCTION__）和委托执行失败的信息。
			// 注意：UE_LOG 的格式化字符串中，%hs 表示 ANSI 字符串（__FUNCTION__ 是编译器宏，返回当前函数名）。
			UE_LOG(AuraLog, Error, TEXT("Failed to execute delegate in function [%hs]"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.Ability.Get()->AbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagByAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStateTagByAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Spell.State"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && AbilitySpec.Ability->AbilityTags.HasTagExact(AbilityTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		const int32 AttributePoint = IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor());
		if (AttributePoint > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdataAbilitiesState(int InLevel)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	if (AbilityInfo == nullptr)return;
	
	for (auto& Info : AbilityInfo->AbilityInfomation)
	{
		if (Info.RequiredLevel <= InLevel)
		{
			if (GetAbilitySpecFromAbilityTag(Info.AbilityTag) ==nullptr)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
				AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Spell_State_Eligible);
				GiveAbility(AbilitySpec);

				ClientAbilityStateChange(Info.AbilityTag, FAuraGameplayTags::Get().Spell_State_Eligible);
				MarkAbilitySpecDirty(AbilitySpec);
			}
		}
	}
}

void UAuraAbilitySystemComponent::ClientAbilityStateChange_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StateTag)
{
	StateChangeDelegate.Broadcast(AbilityTag, StateTag);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	//客户端实现
	if (!bStartAbilityDeleagte)
	{
		bStartAbilityDeleagte = true;
		AbilityDeleagte.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(), -1);
	}
}

