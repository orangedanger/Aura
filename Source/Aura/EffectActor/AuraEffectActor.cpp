// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereCollision->SetupAttachment(Mesh);

}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	//°ó¶¨Åö×²
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlapEnd);
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy") && !bAppliedToEnmey) return;

	if (InstantGameplayEffectClass)
	{
		checkf(InstantGameplayEffectClass, TEXT("Set BP_InstantGameplayEffectClass First"));
		ApplyEffectToTarget(OtherActor, InstantGameplayEffectClass);
		Destroy();
	}

	if (DurationGameplayEffectClass)
	{
		checkf(DurationGameplayEffectClass, TEXT("Set BP_DurationGameplayEffectClass First"));
		ApplyEffectToTarget(OtherActor, DurationGameplayEffectClass);
		Destroy();
	}

	if (InfiniteGameplayEffectClass)
	{
		checkf(InfiniteGameplayEffectClass, TEXT("Set BP_InfiniteGameplayEffectClass First"));
		ApplyEffectToTarget(OtherActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UAbilitySystemComponent* ASCTarget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASCTarget == nullptr)return;

	TArray<FActiveGameplayEffectHandle> RemoveEffectHandleMap;
	for (auto& EffectHandle : ActiveEffectHandleMap)
	{
		if (ASCTarget == EffectHandle.Value)
		{
			EffectHandle.Value->RemoveActiveGameplayEffect(EffectHandle.Key,1);
			RemoveEffectHandleMap.Add(EffectHandle.Key);
		}	
	}

	for (auto& EffectHandle : RemoveEffectHandleMap)
	{
		ActiveEffectHandleMap.FindAndRemoveChecked(EffectHandle);
	}

}

void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (Target->ActorHasTag("Enemy") && !bAppliedToEnmey) return;

	UAbilitySystemComponent* ASCTarget = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (ASCTarget == nullptr)return;

	FGameplayEffectContextHandle EffectContextHandle = ASCTarget->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle EffectSpecHandle = ASCTarget->MakeOutgoingSpec(GameplayEffect, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ASCTarget->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite)
	{
		ActiveEffectHandleMap.Add(ActiveGameplayEffectHandle, ASCTarget);
	}
}
