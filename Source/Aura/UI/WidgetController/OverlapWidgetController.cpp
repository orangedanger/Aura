// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlapWidgetController.h"
#include"Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include"Aura/AbilitySystem/AbilityInfo.h"
#include "Aura/PlayerState/AuraPlayerState.h"
#include"Aura/AuraGameplayTags.h"


//�����ݽ��г�ʼ����ֵ�ĵط�
void UOverlapWidgetController::InitBroadcast()
{
	//����Overlapʱͬʱ��ʼ��
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlapWidgetController::BroadcastCallbackToDependences()
{
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddUObject(this, &UOverlapWidgetController::HealthChange);
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddUObject(this, &UOverlapWidgetController::MaxHealthChange);
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddUObject(this, &UOverlapWidgetController::ManaChange);
	AbilitySubsystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddUObject(this, &UOverlapWidgetController::MaxManaChange);


	if (GetAuraASC())
	{
		//��Lambda���ʽ
		GetAuraASC()->AssertTag.AddLambda(
			//�����this����Lambda�����г������еĳ�Աʱ��Ҫ��
			[this](const FGameplayTagContainer& Container)
			{
				for (const FGameplayTag& Tags : Container)
				{
					//�ж�Tags�Ƿ����� MessageTag
					//"Message.HealthPotion".MatchesTag("Message") will return True, 
					//"Message".MatchesTag("Message.HealthPotion") will return False
					//ɸѡ��Message��Tag
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tags.MatchesTag(MessageTag))
					{
						checkf(MessageDataTable, TEXT("Please Set MessageDataTable in BP_OverlapWidgetController First"));
						//Ѱ�Ҷ�Ӧ��Row
						FUIWidgetData* WidgetData = MessageDataTable->FindRow<FUIWidgetData>(Tags.GetTagName(), TEXT(""));
						//��UE��ͼ���и��¼������䴥��,��������Ӧ��Widget��ͼƬ
						OnMessageWidgetSingnature.Broadcast(*WidgetData);
					}
				}
			}
		);

		//�Ƿ�󶨹�
		if (GetAuraASC()->bStartAbilityDeleagte)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			//Ability��ί��
			GetAuraASC()->AbilityDeleagte.AddUObject(this, &UOverlapWidgetController::BroadcastAbilityInfo);
		}
	}

	if (GetAuraPS())
	{
		GetAuraPS()->OnXpChangedDelegate.AddUObject(this, &UOverlapWidgetController::OnXpChange);
		GetAuraPS()->OnLevelChangeDelegate.AddUObject(this, &UOverlapWidgetController::OnLevelChange);
	}

}

void UOverlapWidgetController::HealthChange(const FOnAttributeChangeData& Date)const
{
	OnHealthChanged.Broadcast(Date.NewValue);
}

void UOverlapWidgetController::MaxHealthChange(const FOnAttributeChangeData& Date)const
{
	OnMaxHealthChanged.Broadcast(Date.NewValue);
}

void UOverlapWidgetController::ManaChange(const FOnAttributeChangeData& Date) const
{
	OnManaChanged.Broadcast(Date.NewValue);
}

void UOverlapWidgetController::MaxManaChange(const FOnAttributeChangeData& Date) const
{
	OnMaxManaChanged.Broadcast(Date.NewValue);
}

void UOverlapWidgetController::OnXpChange(int32 NewXp)
{
	//��ȡ���ڵȼ�
	UXpRewardData* XpRewardData = GetAuraPS()->XpData;
	checkf(XpRewardData, TEXT("Please Put XpData In BP_AuraPlayerState"));

	float Percent;
	const int32 Level = XpRewardData->FindLevelAtXp(NewXp);

	//����ȼ��Ѿ��������
	if (Level == XpRewardData->XpRewardDataArray.Num())
	{
		Percent = 1.f;
	}
	else
	{
		const int32 CurrentLevelXp = XpRewardData->XpRewardDataArray[Level - 1].RequiredXp;
		const int32 RequiredLevelXp = XpRewardData->XpRewardDataArray[Level].RequiredXp;
		const int32 CurrentXp = NewXp - CurrentLevelXp;
		const int32 RequiredXp = RequiredLevelXp - CurrentLevelXp;
		Percent = static_cast<float>(CurrentXp) / static_cast<float>(RequiredXp);
	}

	OnXpPercentChangeSingnature.Broadcast(Percent);
}

void UOverlapWidgetController::OnLevelChange(int32 NewLevel)
{
	OnLevelChangeDelegate.Broadcast(NewLevel);
}
