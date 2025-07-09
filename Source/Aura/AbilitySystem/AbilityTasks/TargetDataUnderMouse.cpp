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
		//�ɱ��ؿ��ƴ���Data�������
		SendMouseCursorData();
	}
	else
	{
		//����������Data������Ӧ
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		//�󶨵ĺ����ں�ʱ���ã� �ȴ���ɺ�ִ��
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//����δ�ɹ����ݣ���Ҫ�ȴ�
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

	//����Data�������
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//��Locationͨ��ί�д��ݸ���ͼ
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	////ʹ�����Կͻ��˵Ļ��� TargetData���� TargetData��
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//��Locationͨ��ί�д��ݸ���ͼ
		ValidData.Broadcast(DataHandle);
	}
}
