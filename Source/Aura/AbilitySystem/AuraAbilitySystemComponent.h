// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FAssertTagDelegate, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FDataInfoDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAbilityStateChangeDelegate, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StateTag*/);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	void AbilityActorInfoSet();

	FAssertTagDelegate AssertTag;
	FDataInfoDelegate AbilityDeleagte;
	FAbilityStateChangeDelegate StateChangeDelegate;

	bool bStartAbilityDeleagte = false;

	void GrantGameplayAbilities(const TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities);
	void GrantGameplayPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);


	/**
	 * �������пɼ���� GameplayAbility������ÿ������ִ�д����ί�С�
	 *
	 * @param Delegate ����һ�� FGameplayAbilitySpec �����Ļص����������ڴ���ÿ��������
	 *                 ��ί�з��� `false`����ֹͣ������
	 */
	void ForEachAbility(FForEachAbility Delegate);

	static FGameplayTag GetAbilityTagByAbilitySpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetInputTagByAbilitySpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetStateTagByAbilitySpec(const FGameplayAbilitySpec& Spec);

	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	void UpgradeSpellPoint();
	void UpdataAbilitiesState(int InLevel);

protected:
	UFUNCTION(Client,Reliable)
	void ClientGameplayEffectAppliedDelegateToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle);

	UFUNCTION(Client, Reliable)
	void ClientAbilityStateChange(const FGameplayTag& AbilityTag, const FGameplayTag& StateTag);

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
};
