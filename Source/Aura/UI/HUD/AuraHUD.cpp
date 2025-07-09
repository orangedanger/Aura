// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraHUD.h"

#include "Aura/UI/UserWidget/AuraUserWidget.h"
#include "Aura/UI/WidgetController/OverlapWidgetController.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/SpellMenuWidgetController.h"

UOverlapWidgetController* AAuraHUD::GetOverlapWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlapWidgetController == nullptr)
	{
		OverlapWidgetController = NewObject<UOverlapWidgetController>(this,OverlapWidgetControllerClass);
		OverlapWidgetController->SetWidgetControllerParams(WCParams);
		OverlapWidgetController->BroadcastCallbackToDependences();
		return OverlapWidgetController;
	}
	return OverlapWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BroadcastCallbackToDependences();
		return AttributeMenuWidgetController;
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BroadcastCallbackToDependences();
		return SpellMenuWidgetController;
	}
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlap(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	/*
	*先检查两个Class是否有值checkf()
	*对UserWidget初始化
	*将UserWidget转化为AuraUserWidget
	*创建OverlapWidgetController并进行初始化
	*并调用AuraUserWidget里面的SetWidgetController进行联系Controller
	**/
	
	checkf(OverlapWidgetClass, TEXT("Set WBP_AuraOverlap To OverlapWidgetClass In BP_AuraHUD"));
	checkf(OverlapWidgetControllerClass, TEXT("Set OverlapWidgetControllerClass In BP_AuraHUD"));
	
	//将创建好的菜单(WBP_AuraOverlap)加入屏幕中
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlapWidgetClass);
	OverlapWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams ControllerParmas(PC, PS, ASC, AS);
	UOverlapWidgetController* OverlapWigetController = GetOverlapWidgetController(ControllerParmas);
	OverlapWidget->SetWidgetController(OverlapWigetController);
	OverlapWigetController->InitBroadcast();


	Widget->AddToViewport();
}
