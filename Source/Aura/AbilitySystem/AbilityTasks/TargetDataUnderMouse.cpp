// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateUnderMouseTargetData(UGameplayAbility* OwningAbility)
{

	UTargetDataUnderMouse* MyObj = NewAbilityTask< UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		//由本地控制传递Data给服务端
		SendMouseCursorData();
	}
	else
	{
		//服务器接受Data做出反应
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		//绑定的函数在何时调用？ 等待完成后执行
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//数据未成功传递，需要等待
		if (!AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey))
		{
			SetWaitingOnRemotePlayerData();
		}

	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC)return;

	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	DataHandle.Add(Data);

	//传递Data到服务端
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//将Location通过委托传递给蓝图
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	////使用来自客户端的缓存 TargetData（仅 TargetData）
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//将Location通过委托传递给蓝图
		ValidData.Broadcast(DataHandle);
	}
}
