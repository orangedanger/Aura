// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraSummonGameplayAbility.generated.h"


class AEnemyCharacter;

// 更高效且可读的数据存储
struct FLocationConstraint {
	float DistanceToOrigin; // 到原点的距离
	float GeneratedAngle;   // 生成时的角度
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetRandomSummonLocation();

	UFUNCTION(BlueprintPure, Category = "Summon")
	TSubclassOf<AEnemyCharacter> GetRandomCHaracterClass();
	
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	int32 NumToSummon = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	TArray<TSubclassOf<AEnemyCharacter>> SummonCharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MinRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MaxRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MaxAngle = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float SpawnMinSpace = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float SpawnMinAngle = 10.f;

	
};
