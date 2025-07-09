// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAIController.h"
#include"BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTree Component");
	check(BehaviorTreeComponent);

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	check(Blackboard);

}
