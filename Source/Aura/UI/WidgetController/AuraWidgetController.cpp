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
	//TOOD: 需要对Data中的InputTag进行设置,需要激活的能力，

	FForEachAbility BroadcastDelegate;
	//绑定 (发送Info数据到蓝图中) 这个功能到BroadcastDelegate
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo info = AbilityInfo.Get()->FindAbilityInfoByAbilityTag(AuraAbilitySystemComponent->GetAbilityTagByAbilitySpec(AbilitySpec));
			info.InputTag = AuraAbilitySystemComponent->GetInputTagByAbilitySpec(AbilitySpec);
			//将Info数据发送到蓝图中
			OnAbilityInfoSingnature.Broadcast(info);
		});
	//执行这个委托的功能
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
