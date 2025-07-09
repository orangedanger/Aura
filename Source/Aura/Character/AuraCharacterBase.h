// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Aura/InterAction/CombatInterface.h"
#include "Aura/AbilitySystem/CharacterClassInfo.h"

#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UFloatingTextWidgetComponent;

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()
public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributSet() const { return AttributeSet; }

	/*
	* Combat Interface
	*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FVector GetWeaponSocketLocation_Implementation(FGameplayTag MontageTag) override;
	virtual bool isDie_Implementation()const override;
	virtual AActor* GetAvatar_Implementation()override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual void SetMinionCount_Implementation(int32 Amout)override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual int32 GetPlayerLevel_Implementation()override;
	
	/**
	*  Death
	*/
	virtual void Death() override;
	UFUNCTION(NetMulticast,Reliable)
	void MulticastDeath();


	/***
	*  Dissolve
	*/
	virtual void Dissolve() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveMeshAndWeaponTimeLine(UMaterialInstanceDynamic* DissolveMeshMaterial, UMaterialInstanceDynamic* DissolveWeaponMeshMaterial);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dissolve")
	TObjectPtr<UMaterialInterface> DissolveMeshMaterialInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dissolve")
	TObjectPtr<UMaterialInterface> DissolveWeaponMeshMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName TailSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponement;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> PrimaryEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> VitalEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> SecondaryEffectClass;

	bool bDeath = false;

	virtual void InitialAbilityActor();

	virtual void InitialAttribute() const;
	void InitialGameEffect(TSubclassOf<UGameplayEffect> EffectClass) const;
	void GrantGameplayAbilities();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharaterClass = ECharacterClass::Warrior;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray <TSubclassOf<UGameplayAbility>> GameplayAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray <TSubclassOf<UGameplayAbility>> GameplayPassiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* DeathSound;
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	int32 MinionCount = 0;

};
