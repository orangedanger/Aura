// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
public:

	AAuraAIController();


protected:



private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	

};
