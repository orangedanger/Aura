// CooldownAsyncTask.h

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "CooldownAsyncTask.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownAsyncTask, float, RemainingTime);

/**
 * UCooldownAsyncTask
 *
 * 该类处理与能力系统相关的冷却时间的异步任务。
 * 它提供了等待冷却时间开始和结束的功能。
 */
UCLASS(BlueprintType,meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UCooldownAsyncTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * 等待冷却时间的开始。
	 *
	 * @param AbilitySystemComponent 要监控的能力系统组件。
	 * @param InCooldownTag 表示冷却时间的游戏标签。
	 * @return 创建的 UCooldownAsyncTask 实例的指针。
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UCooldownAsyncTask* WaitforCooldownStart(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag InCooldownTag);

	/** 冷却时间开始时触发的事件。 */
	UPROPERTY(BlueprintAssignable)
	FOnCooldownAsyncTask OnCooldownStart;

	/** 冷却时间结束时触发的事件。 */
	UPROPERTY(BlueprintAssignable)
	FOnCooldownAsyncTask OnCooldownEnd;

	/** 结束异步任务。 */
	UFUNCTION(BlueprintCallable)
	void AsyncTaskEnd();


private:
	/**
	 * 当游戏效果应用到自身时调用。
	 *
	 * @param AbilitySystemComponent 能力系统组件。
	 * @param GameSpec 游戏效果规格。
	 * @param GameplayHandle 活动游戏效果句柄。
	 */
	void GameplayEffectApplyToSelf(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameSpec, FActiveGameplayEffectHandle GameplayHandle);
	
	/**
	 * 当游戏标签事件发生时调用。
	 *
	 * @param InCooldownTag 游戏标签。
	 * @param NewCount 标签的新计数。
	 */
	void GameplayTagEvent(const FGameplayTag InCooldownTag, int32 NewCount);

	/** 被监控的能力系统组件。 */
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	/** 表示冷却时间的游戏标签。 */
	FGameplayTag CooldownTag;
};

