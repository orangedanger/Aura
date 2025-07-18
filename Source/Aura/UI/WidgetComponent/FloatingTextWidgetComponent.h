// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingTextWidgetComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UFloatingTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFloatingText(float DamageText, bool bCritical, bool bEvade);
};
