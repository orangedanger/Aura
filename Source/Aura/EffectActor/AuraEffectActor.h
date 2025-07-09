// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "AuraEffectActor.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UGameplayEffect;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


	void ApplyEffectToTarget(AActor* Target,TSubclassOf<UGameplayEffect> GameplayEffect);
private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandleMap;

	UPROPERTY(EditAnywhere)
	float ActorLevel = 1.f;

	UPROPERTY(EditAnywhere)
	bool bAppliedToEnmey = false;
};
