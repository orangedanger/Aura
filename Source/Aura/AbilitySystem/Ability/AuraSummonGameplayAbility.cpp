// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraSummonGameplayAbility.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Aura/Character/EnemyCharacter.h"

TArray<FVector> UAuraSummonGameplayAbility::GetRandomSummonLocation() 
{
    TArray<FVector> SpawnLocations;

    // ����Actor����
    const AActor* AvatarActor = GetAvatarActorFromActorInfo();
    const FVector ActorLocation = AvatarActor->GetActorLocation();
    const FVector ActorForward = AvatarActor->GetActorForwardVector();

    // Ԥ���㳣��
    const float HalfAngle = MaxAngle * 0.5f;
    const FVector LeftDir = ActorForward.RotateAngleAxis(HalfAngle, FVector::UpVector);
    const FVector RightDir = ActorForward.RotateAngleAxis(-HalfAngle, FVector::UpVector);

    // ���Ի���
//#if WITH_EDITOR
//    UKismetSystemLibrary::DrawDebugLine(this, ActorLocation + MinRadius * LeftDir, ActorLocation + MaxRadius * LeftDir, FColor::Green, 3.f);
//    UKismetSystemLibrary::DrawDebugLine(this, ActorLocation + MinRadius * RightDir, ActorLocation + MaxRadius * RightDir, FColor::Green, 3.f);
//#endif

    TArray<FLocationConstraint> ExistingConstraints;
    const int32 MaxAttempts = NumToSummon * 10; // ��ֹ����ѭ��
    int32 Attempts = 0;

    while (SpawnLocations.Num() < NumToSummon && Attempts++ < MaxAttempts) 
    {
        // ���ɺ�ѡλ��
        const float Radius = FMath::FRandRange(MinRadius, MaxRadius);
        const float Angle = FMath::FRandRange(-HalfAngle, HalfAngle);
        const FVector Direction = ActorForward.RotateAngleAxis(Angle, FVector::UpVector);
        FVector CandidateLocation = ActorLocation + Radius * Direction;

        // У������
        bool bValid = true;
        for (const FLocationConstraint& Constraint : ExistingConstraints) 
        {
            const FVector ExistingLocation = ActorLocation + Constraint.DistanceToOrigin * ActorForward.RotateAngleAxis(Constraint.GeneratedAngle, FVector::UpVector);
            const float Distance = FVector::Distance(CandidateLocation, ExistingLocation);
            const float AngularDiff =FMath::Abs(Constraint.GeneratedAngle - Angle)/* ʵ�ʽǶȲ���� */;

            if (Distance < SpawnMinSpace && AngularDiff < SpawnMinAngle) 
            {
                bValid = false;
                break;
            }
        }

		if (bValid)
		{
            FHitResult Hit;
            GetWorld()->LineTraceSingleByChannel(Hit, CandidateLocation + FVector(0, 0, 150.f), CandidateLocation - FVector(0, 0, 150.f), ECC_Visibility);
            CandidateLocation = Hit.Location;

			ExistingConstraints.Add({ Radius, Angle });
			SpawnLocations.Add(CandidateLocation);
//#if WITH_EDITOR
//			UKismetSystemLibrary::DrawDebugSphere(this, CandidateLocation, 30.f, 12, FColor::Blue, 3.f);
//#endif
		}
    }

    return SpawnLocations;
}

TSubclassOf<AEnemyCharacter> UAuraSummonGameplayAbility::GetRandomCHaracterClass()
{

    int32 RandomIndex = FMath::RandRange(0, SummonCharacterClass.Num() - 1);
    return SummonCharacterClass[RandomIndex];
}
