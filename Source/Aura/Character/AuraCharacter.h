// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/InterAction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UNiagaraComponent;

/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void BeginPlay()override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;



	virtual int32 GetPlayerLevel_Implementation() override;

	/*
	*	Player Interface
	*/

	virtual void AddToXp_Implementation(int32 Xp) override;
	virtual int32 FindLevelForXp_Implementation(int32 Xp) override;
	virtual int32 GetPlayerXp_Implementation() override;
	virtual void LevelUp_Implementation(int32 UpValue) override;
	virtual int32 GetAttributePointReward_Implementation(int32 Level) override;
	virtual int32 GetShellPointReward_Implementation(int32 Level) override;
	virtual void AddToAttributePoint_Implementation(int32 AttributePoint) override;
	virtual void AddToShellPoint_Implementation(int32 ShellPoint) override;
	virtual int32 GetAttributePoint_Implementation() override;
	virtual int32 GetShellPoint_Implementation() override;


protected:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelupEffect();

private:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, Category = "Level up")
	TObjectPtr<UNiagaraComponent> LevelupEffect;


	virtual void InitialAbilityActor() override;

};
