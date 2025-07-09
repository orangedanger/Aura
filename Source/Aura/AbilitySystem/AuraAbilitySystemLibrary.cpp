// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraAbilitySystemLibrary.h"
#include "Aura/UI/WidgetController/OverlapWidgetController.h"

#include "Aura/UI/WidgetController/SpellMenuWidgetController.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/PlayerController/AuraPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/GameMode/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "Aura/AuraAbilityTypes.h"
#include "Aura/InterAction/CombatInterface.h"
#include "Aura/LogCategory.h"
#include "Engine\OverlapResult.h"


bool UAuraAbilitySystemLibrary::GetWidgetControllerParams(const UObject* WorldContext, FWidgetControllerParams& OutParams, AAuraHUD*& OutHUD)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContext, 0)))
	{
		OutHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutHUD)
		{
			if (AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributSet();
				OutParams = FWidgetControllerParams(PC, PS, ASC, AS);
				return true;
			}
		}
	}
	return false;
}

UOverlapWidgetController* UAuraAbilitySystemLibrary::GetOverlapWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams Params;
	AAuraHUD* AuraHUD = nullptr;
	bool bHasParams = GetWidgetControllerParams(WorldContext, Params, AuraHUD);
	if (bHasParams)
	{
		return AuraHUD->GetOverlapWidgetController(Params);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams Params;
	AAuraHUD* AuraHUD = nullptr;
	bool bHasParams = GetWidgetControllerParams(WorldContext, Params, AuraHUD);
	if (bHasParams)
	{
		return AuraHUD->GetAttributeMenuWidgetController(Params);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams Params;
	AAuraHUD* AuraHUD = nullptr;
	bool bHasParams = GetWidgetControllerParams(WorldContext, Params, AuraHUD);
	if (bHasParams)
	{
		return AuraHUD->GetSpellMenuWidgetController(Params);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitDefaultAttribute(const UObject* WorldContext, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);

	if (ClassInfo == nullptr || ASC == nullptr)return;

	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);

	const AActor* AvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle PrimarySpecHandle = ASC->MakeEffectContext();
	PrimarySpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, level, PrimarySpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondarySpecHandle = ASC->MakeEffectContext();
	SecondarySpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, level, SecondarySpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalSpecHandle = ASC->MakeEffectContext();
	VitalSpecHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, level, VitalSpecHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}


UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContext)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (AuraGameMode)
	{
		return AuraGameMode->AbilityClassInfo;
	}
	return nullptr;
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (AuraGameMode)
	{
		return AuraGameMode->CharacterClassInfo;
	}
	return nullptr;
}


void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);

	if (ClassInfo == nullptr || ASC == nullptr)return;

	for (TSubclassOf<UGameplayAbility> AbilityClassOf : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	//获取Abilities数组
	const FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);
	//在CombatInterface中可以获取Level
	if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		//遍历去初始化每个Ability
		for (TSubclassOf<UGameplayAbility> AbilityClassOf : DefaultInfo.SetupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClassOf,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}

}

bool UAuraAbilitySystemLibrary::IsCriticalHIt(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		return AuraEffectContext->IsCritical();
	}
	else
	{
		return false;
	}
}

bool UAuraAbilitySystemLibrary::IsEvade(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		return AuraEffectContext->IsEvade();
	}
	else
	{
		return false;
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit)
{
	FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsCritical(bCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsEvade(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bEvade)
{
	FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (AuraEffectContext)
	{
		AuraEffectContext->SetIsEvade(bEvade);
	}
}

void UAuraAbilitySystemLibrary::QueryActorsInRadius(const UObject* WorldContext, TArray<AActor*>& OutputActor, const TArray<AActor*>& IgnoreActors, const float Radius, const FVector SphereCenter)
{
	FCollisionQueryParams SphereParams;

	SphereParams.AddIgnoredActors(IgnoreActors);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereCenter, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (const FOverlapResult& Overlap:Overlaps)
		{
			//检测Overlap角色是否继承了 CombatInterface类 且 是否死亡
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_isDie(Overlap.GetActor()))
			{
				OutputActor.AddUnique(Overlap.GetActor());
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriends(const AActor* SoureActor, const AActor* TargetActor)
{
	bool IsAllPlayer =SoureActor->ActorHasTag(FName("Player")) &&TargetActor->ActorHasTag(FName("Player"));
	bool IsAllEnemy = SoureActor->ActorHasTag(FName("Enemy")) && TargetActor->ActorHasTag(FName("Enemy"));
	return !(IsAllPlayer||IsAllEnemy);
}

FTaggedMontage UAuraAbilitySystemLibrary::GetRandomMontageFromArray(const TArray<FTaggedMontage>& Montages)
{
	if (Montages.Num() > 0)
	{
		return Montages[FMath::RandRange(0, Montages.Num() - 1)];
	}
	return FTaggedMontage();
}

FTaggedMontage UAuraAbilitySystemLibrary::GetMontageFromArrayByTag(const TArray<FTaggedMontage>& Montages, FGameplayTag MontageTag)
{
	for (const auto& Montage :Montages)
	{
		if (Montage.MontageTag == MontageTag)
		{
			return Montage;
		}	
	}
	return FTaggedMontage();
}

int32 UAuraAbilitySystemLibrary::GetRewardXpForClassAndLevel(const UObject* WorldContext, float Level, ECharacterClass InClass)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext);
	if (ClassInfo == nullptr)	return 0;

	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterDefaultInfo(InClass);

	const int32 RewardXp = ClassDefaultInfo.XpReward.GetValueAtLevel(Level);

	return RewardXp;
}
