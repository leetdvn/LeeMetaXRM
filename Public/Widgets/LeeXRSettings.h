// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeeXRSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LEEMETAXRM_API ULeeXRSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULeeXRSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

};
