// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Aura/InterAction/EnemyInterface.h"
#include "Aura/InterAction/CombatInterface.h"
#include "Aura/UI/WidgetController/OverlapWidgetController.h"
#include "Aura/AbilitySystem/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class AAuraAIController;
class UBehaviorTree;
/**
 *
 */
UCLASS()
class AURA_API AEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	virtual void BeginPlay()override;

	virtual void PossessedBy(AController* NewController) override;
	/**
	* Enemy Interface
	*/
	virtual void HighLightAction()override;
	virtual void UnHighLightAction()override;


	virtual void SetCombatTarget_Implementation(AActor* CombatActor) override;
	virtual AActor* GetCombatTarget_Implementation() override;

	/**
	* Combat Interface
	*/
	virtual int32 GetPlayerLevel_Implementation() override;


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnMaxHealthChange;

	virtual void Death() override;

protected:

	virtual void InitialAbilityActor() override;

	virtual void InitialAttribute() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
private:
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseSpeed = 250.f;

};
