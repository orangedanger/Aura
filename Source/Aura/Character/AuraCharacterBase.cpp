// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/Aura.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/AuraGameplayTags.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_ProjectileChannel, ECollisionResponse::ECR_Overlap);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetWeaponSocketLocation_Implementation(FGameplayTag MontageTag)
{
	if (FAuraGameplayTags::Get().Montage_Attack_Weapon == MontageTag)
	{
		return WeaponMesh->GetSocketLocation(WeaponSocketName);
	}
	if (FAuraGameplayTags::Get().Montage_Attack_LeftHand == MontageTag)
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (FAuraGameplayTags::Get().Montage_Attack_RightHand == MontageTag)
	{
		return  GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (FAuraGameplayTags::Get().Montage_Attack_Tail == MontageTag)
	{
		return  GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

bool AAuraCharacterBase::isDie_Implementation() const
{
	return bDeath;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return AttackMontages;
}

int32 AAuraCharacterBase::GetMinionCount_Implementation() const
{
	return MinionCount;
}

void AAuraCharacterBase::SetMinionCount_Implementation(int32 Amout)
{
	MinionCount += Amout;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharaterClass;
}

void AAuraCharacterBase::InitialAbilityActor()
{
}

void AAuraCharacterBase::InitialAttribute() const
{
	checkf(PrimaryEffectClass, TEXT("Please check  Character Blueprint is add PrimaryEffectClass Blueprint"));
	InitialGameEffect(PrimaryEffectClass);

	checkf(VitalEffectClass, TEXT("Please check  Character Blueprint is add VitalEffectClass Blueprint"));
	InitialGameEffect(VitalEffectClass);

	checkf(SecondaryEffectClass, TEXT("Please check  Character Blueprint is add SecondaryEffectClass Blueprint"));
	InitialGameEffect(SecondaryEffectClass);
}

void AAuraCharacterBase::InitialGameEffect(TSubclassOf<UGameplayEffect> EffectClass) const
{
	if (IsValid(GetAbilitySystemComponent()))
	{
		FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, 1.f, EffectContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
	}
}

void AAuraCharacterBase::GrantGameplayAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponement);

	//Ability必须在服务端上被授权
	if (!HasAuthority())
	{
		return;
	}

	AuraASC->GrantGameplayAbilities(GameplayAbilities);
	AuraASC->GrantGameplayPassiveAbilities(GameplayPassiveAbilities);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponement;
}

int32 AAuraCharacterBase::GetPlayerLevel_Implementation()
{
	return 0;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Death()
{
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastDeath();
}

void AAuraCharacterBase::MulticastDeath_Implementation()
{
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetSimulatePhysics(true);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	

	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	Dissolve();
	bDeath = true;
}

void AAuraCharacterBase::Dissolve()
{
	UMaterialInstanceDynamic* MeshMaterial = nullptr;
	UMaterialInstanceDynamic* WeaponMeshMaterial = nullptr;
	if (IsValid(DissolveMeshMaterialInstance))
	{
		MeshMaterial = UMaterialInstanceDynamic::Create(DissolveMeshMaterialInstance, this);
		GetMesh()->SetMaterial(0, MeshMaterial);
	}

	if (IsValid(DissolveWeaponMeshMaterialInstance))
	{
		WeaponMeshMaterial = UMaterialInstanceDynamic::Create(DissolveWeaponMeshMaterialInstance, this);
		WeaponMesh->SetMaterial(0, WeaponMeshMaterial);
	}

	StartDissolveMeshAndWeaponTimeLine(MeshMaterial, WeaponMeshMaterial);
}