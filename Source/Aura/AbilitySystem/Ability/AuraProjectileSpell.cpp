// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraProjectileSpell.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/Actor/AuraProjectile.h"
#include "Aura/InterAction/CombatInterface.h"
#include "Aura/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::ProjectileShell(const FVector& HitLocation,const FGameplayTag& WeaponTag)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	FTransform ProjectileTransform;

	if (!GetAvatarActorFromActorInfo()->Implements<UCombatInterface>()) return;
	const FVector SocketLocation = ICombatInterface::Execute_GetWeaponSocketLocation(GetAvatarActorFromActorInfo(), WeaponTag);
	
	ProjectileTransform.SetLocation(SocketLocation);

	FRotator Rotation = (HitLocation - SocketLocation).Rotation();
	//Rotation.Pitch = 0.f;
	ProjectileTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* AuraProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		ProjectileTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	UAbilitySystemComponent* SoureASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (SoureASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = SoureASC->MakeEffectContext();

		EffectContextHandle.AddSourceObject(AuraProjectile);
		EffectContextHandle.SetAbility(this);
		TArray<TWeakObjectPtr<AActor>> InActors;
		InActors.Add(AuraProjectile);
		EffectContextHandle.AddActors(InActors);

		FHitResult HitResult;
		EffectContextHandle.AddHitResult(HitResult);

		AuraProjectile->DamageSpecHandle = SoureASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	}

	//float Damage = 0.f;
	for (auto& pair : Damages)
	{
		const float ScaleDamage = pair.Value.GetValueAtLevel(GetAbilityLevel());
		//将Attribute 和 Tag 和 伤害数值结合
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(AuraProjectile->DamageSpecHandle, pair.Key, ScaleDamage);

		//Damage += ScaleDamage;
	}

	//UE_LOG(LogTemp, Warning, TEXT("ScaleDamage : %f"), ScaleDamage);

	AuraProjectile->FinishSpawning(ProjectileTransform);
}