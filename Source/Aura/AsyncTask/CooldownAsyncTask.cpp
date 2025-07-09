// CooldownAsyncTask.cpp

#include "CooldownAsyncTask.h"
#include "AbilitySystemComponent.h"

UCooldownAsyncTask* UCooldownAsyncTask::WaitforCooldownStart(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCooldownTag)
{
	UCooldownAsyncTask* CooldownAsyncTask = NewObject<UCooldownAsyncTask>();
	if (!AbilitySystemComponent || !InCooldownTag.IsValid())
	{
		CooldownAsyncTask->AsyncTaskEnd();
		return nullptr;
	}

	CooldownAsyncTask->ASC = AbilitySystemComponent;
	CooldownAsyncTask->CooldownTag = InCooldownTag;

	// 绑定到能力激活时触发的委托（CooldownStart）
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(CooldownAsyncTask, &UCooldownAsyncTask::GameplayEffectApplyToSelf);

	// 绑定到标签增加或移除时触发的委托（CooldownEnd）
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(CooldownAsyncTask, &UCooldownAsyncTask::GameplayTagEvent);

	return CooldownAsyncTask;
}

/** 结束异步任务。 */
void UCooldownAsyncTask::AsyncTaskEnd()
{
	if (ASC)
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCooldownAsyncTask::GameplayEffectApplyToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle)
{
	FGameplayTagContainer AssetTags;
	GameSpec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantTags;
	GameSpec.GetAllGrantedTags(GrantTags);
	
	// 检查游戏效果是否为冷却效果
	if (AssetTags.HasTagExact(CooldownTag) || GrantTags.HasTagExact(CooldownTag))
	{
		// 获取剩余冷却时间
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> CooldownTimes = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);

		if (CooldownTimes.Num() > 0)
		{
			// 获取最大冷却时间
			float MaxCooldownTime = CooldownTimes[0];
			for (int i = 0; i < CooldownTimes.Num(); i++)
			{
				MaxCooldownTime = FMath::Max(MaxCooldownTime, CooldownTimes[i]);
			}

			// 广播冷却时间开始事件
			OnCooldownStart.Broadcast(MaxCooldownTime);
		}
	}
}

void UCooldownAsyncTask::GameplayTagEvent(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		// 广播冷却时间结束事件
		OnCooldownEnd.Broadcast(0.f);
	}
}

