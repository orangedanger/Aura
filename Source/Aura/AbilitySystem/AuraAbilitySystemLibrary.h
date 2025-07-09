// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Aura/AbilitySystem/CharacterClassInfo.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UOverlapWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
struct FWidgetControllerParams;
class AAuraHUD;

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static bool GetWidgetControllerParams(const UObject* WorldContext, FWidgetControllerParams& OutParams, AAuraHUD*& OutHUD);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary", meta = (DefaultToSelf = "WorldContext"))
	static UOverlapWidgetController* GetOverlapWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary", meta = (DefaultToSelf = "WorldContext"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary", meta = (DefaultToSelf = "WorldContext"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary", meta = (DefaultToSelf = "WorldContext"))
	static void InitDefaultAttribute(const UObject* WorldContext, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC);

	//初始化Character Class Info 中的几个 Abilities
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilitySystemLibrary")
	static void GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static bool IsCriticalHIt(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static bool IsEvade(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameEffectContext")
	static void SetIsEvade(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bEvade);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|QueryActors")
	static void QueryActorsInRadius(const UObject* WorldContext, TArray<AActor*>& OutputActor, const TArray<AActor*>& IgnoreActors,const float Radius,const FVector SphereCenter);

	UFUNCTION(BlueprintCallable,Category = "AuraAbilitySystemLibrary|QueryActors")
	static bool IsNotFriends(const AActor* SoureActor,const AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Montage")
	static FTaggedMontage GetRandomMontageFromArray(const TArray<FTaggedMontage>& Montages);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Montage")
	static FTaggedMontage GetMontageFromArrayByTag(const TArray<FTaggedMontage>& Montages,FGameplayTag MontageTag);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Xp")
	static int32 GetRewardXpForClassAndLevel(const UObject* WorldContext,float Level,ECharacterClass InClass);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);

};
