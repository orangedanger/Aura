// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellPointSignature, int32, SpellPoint);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void InitBroadcast() override;
	virtual void BroadcastCallbackToDependences()override;

	UPROPERTY(BlueprintAssignable, Category = "SpellMenu|Delegate")
	FOnSpellPointSignature SpellPointDelegate;
};
