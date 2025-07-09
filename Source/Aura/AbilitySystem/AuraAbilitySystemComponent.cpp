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
			//���б�ǩ�İ�,�Զ����Tag���뵽Ability�Ķ�̬��ǩ��
			AbilitySpec.DynamicAbilityTags.AddTag(GameplayAbility->InputGameplayTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Spell_State_Eligible);
			//��Ability�������Ӧ�Ľ�ɫ
			GiveAbility(AbilitySpec);
		}
	}
	//��ʼ��UI Ability��ʾ
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
	// ʹ������������Scoped Lock��������ǰ Ability �б�ȷ�������������б��ᱻ�޸ġ�
	// ����Ϊ�˱����ڵ����������������̻߳��߼��޸� Ability �б��µ����ݾ�����һ���ԡ�
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	// �������пɼ���� Ability��ͨ�� GetActivatableAbilities ��ȡ�б�
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// ִ��ί�У�����ǰ AbilitySpec ��Ϊ�������ݡ�
		// ExecuteIfBound ���� `false` ��ʾί��δ�󶨻�ִ��ʧ�ܡ�
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			// ��¼������־�������ǰ��������__FUNCTION__����ί��ִ��ʧ�ܵ���Ϣ��
			// ע�⣺UE_LOG �ĸ�ʽ���ַ����У�%hs ��ʾ ANSI �ַ�����__FUNCTION__ �Ǳ������꣬���ص�ǰ����������
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

	//�ͻ���ʵ��
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

