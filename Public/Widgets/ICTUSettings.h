// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ICTUSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LEEMETAXRM_API UICTUSettings : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UICTUSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativeConstruct() override;

};
