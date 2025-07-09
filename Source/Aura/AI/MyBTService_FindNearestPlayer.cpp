// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UMyBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = AIOwner->GetPawn();

	//获取敌对目标的Target
	const FName TargetTag = AIPawn->ActorHasTag("Player") ? FName("Enemey") : FName("Player");

	//获取Actors
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(AIPawn, TargetTag, TargetActors);

	float NearestDistance = TNumericLimits<float>::Max();
	AActor* NearestActor = nullptr;
	//找最近的玩家
	for (AActor*& TargetActor : TargetActors)
	{
		const float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
		if (DistanceToTarget < NearestDistance)
		{
			NearestDistance = DistanceToTarget;
			NearestActor = TargetActor;
		}
	}
	//设置Selector对应相应的值
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, NearestDistance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, NearestActor);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, NearestActor->GetName());
}
