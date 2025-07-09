// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Aura/AbilitySystem/XpRewardData.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnXpChangedDelegate, int32);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	UAttributeSet* GetAttributSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerXp() const { return Xp; }
	FORCEINLINE int32 GetAttributePoint() const { return AttributePoint; }
	FORCEINLINE int32 GetShellPoint() const { return ShellPoint; }

	void AddXp(int32 XpToAdd);
	void SetXp(int32 NewXp);
	void SetLevel(int32 NewLevel);
	void AddLevel(int32 LevelsToAdd);

	void AddAttributePoint(int32 AttributePointToAdd);
	void AddShellPoint(int32 ShellPointToAdd);

	UPROPERTY(EditAnywhere, Category = "Player Level")
	TObjectPtr<UXpRewardData> XpData;

	FOnXpChangedDelegate OnXpChangedDelegate;
	FOnXpChangedDelegate OnLevelChangeDelegate;
	FOnXpChangedDelegate OnAttributePointChangeDelegate;
	FOnXpChangedDelegate OnShellPointChangeDelegate;

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponement;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level,Category = "Player Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Xp, Category = "Player Level")
	int32 Xp = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoint, Category = "Player Level")
	int32 AttributePoint = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ShellPoint, Category = "Player Level")
	int32 ShellPoint = 0;


	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_Xp(int32 OldXp);

	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	UFUNCTION()
	void OnRep_ShellPoint(int32 OldShellPoint);

};
