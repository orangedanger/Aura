// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraWidgetController.h"
#include "Aura/PlayerController/AuraPlayerController.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/AbilityInfo.h"


void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySubsystemComponent = WCParams.AbilitySubsystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::InitBroadcast()
{
}

void UAuraWidgetController::BroadcastCallbackToDependences()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraASC()->bStartAbilityDeleagte) return;
	//TOOD: ��Ҫ��Data�е�InputTag��������,��Ҫ�����������

	FForEachAbility BroadcastDelegate;
	//�� (����Info���ݵ���ͼ��) ������ܵ�BroadcastDelegate
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo info = AbilityInfo.Get()->FindAbilityInfoByAbilityTag(AuraAbilitySystemComponent->GetAbilityTagByAbilitySpec(AbilitySpec));
			info.InputTag = AuraAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
			//��Info���ݷ��͵���ͼ��
			OnAbilityInfoSingnature.Broadcast(info);
		});
	//ִ�����ί�еĹ���
	GetAuraASC()->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySubsystemComponent);
	}
	return AuraAbilitySystemComponent;

}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
