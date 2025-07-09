// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayEffectTypes.h"
#include "Aura/AbilitySystem/AbilityInfo.h"
#include "OverlapWidgetController.generated.h"


class UAuraUserWidget;
class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FUIWidgetData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSingnature, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSingnature, int, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetSingnature, FUIWidgetData, MessageWidgetDateTable);


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class AURA_API UOverlapWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:	
	virtual void InitBroadcast() override;
	virtual void BroadcastCallbackToDependences()override;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|UI")
	TObjectPtr<UDataTable> MessageDataTable;
private:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSingnature OnMaxManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnMessageWidgetSingnature OnMessageWidgetSingnature;


	UPROPERTY(BlueprintAssignable, Category = "GAS|Xp")
	FOnAttributeChangedSingnature OnXpPercentChangeSingnature;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Xp")
	FOnLevelChangedSingnature OnLevelChangeDelegate;
private:
	void HealthChange(const FOnAttributeChangeData& Date)const ;

	void MaxHealthChange(const FOnAttributeChangeData& Date)const;

	void ManaChange(const FOnAttributeChangeData& Date)const;

	void MaxManaChange(const FOnAttributeChangeData& Date)const;

	void OnXpChange(int32 NewXp);

	void OnLevelChange(int32 NewLevel);
};

//template <typename T>
//T* GetWidgetDataByTagName(UDataTable* DataTable, const FGameplayTag Tag)
//{
//	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
//}
//
