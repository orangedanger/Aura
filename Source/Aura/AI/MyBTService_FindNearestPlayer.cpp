// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UMyBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = AIOwner->GetPawn();

	//��ȡ�ж�Ŀ���Target
	const FName TargetTag = AIPawn->ActorHasTag("Player") ? FName("Enemey") : FName("Player");

	//��ȡActors
	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(AIPawn, TargetTag, TargetActors);

	float NearestDistance = TNumericLimits<float>::Max();
	AActor* NearestActor = nullptr;
	//����������
	for (AActor*& TargetActor : TargetActors)
	{
		const float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
		if (DistanceToTarget < NearestDistance)
		{
			NearestDistance = DistanceToTarget;
			NearestActor = TargetActor;
		}
	}
	//����Selector��Ӧ��Ӧ��ֵ
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, NearestDistance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, NearestActor);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, NearestActor->GetName());
}
