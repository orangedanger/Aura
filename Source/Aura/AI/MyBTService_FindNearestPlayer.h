// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "MyBTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMyBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
public:


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;



	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector DistanceToTargetSelector;
private:
	
};
