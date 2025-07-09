// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraPlayerState.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;
	AbilitySystemComponement = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponement->SetIsReplicated(true);
	AbilitySystemComponement->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, Xp);
}

void AAuraPlayerState::AddXp(int32 XpToAdd)
{
	Xp += XpToAdd;
	OnXpChangedDelegate.Broadcast(Xp);

}

void AAuraPlayerState::SetXp(int32 NewXp)
{
	Xp = NewXp;
	OnXpChangedDelegate.Broadcast(Xp);
}

void AAuraPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddLevel(int32 LevelsToAdd)
{
	Level += LevelsToAdd;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddAttributePoint(int32 AttributePointToAdd)
{
	AttributePoint += AttributePointToAdd;
	OnAttributePointChangeDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::AddShellPoint(int32 ShellPointToAdd)
{
	ShellPoint += ShellPointToAdd;
	OnShellPointChangeDelegate.Broadcast(ShellPoint);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_Xp(int32 OldXp)
{
	OnXpChangedDelegate.Broadcast(Xp);
}

void AAuraPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChangeDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::OnRep_ShellPoint(int32 OldShellPoint)
{
	OnShellPointChangeDelegate.Broadcast(ShellPoint);
}



UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponement;
}
