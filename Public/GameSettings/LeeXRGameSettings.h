// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LeeXRGameSettings.generated.h"

/**
 * 
 */
UCLASS(config = GameUserSettings, configdonotcheckdefaults, Blueprintable)
class LEEMETAXRM_API ULeeXRGameSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	ULeeXRGameSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	// Sets the anti-aliasing method 
	// @param Value 0:none, 1:FXAA, 2:TAA, 3:MSAA, 4:TSR 
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetAntiAliasingMethod(EAntiAliasingMethod Value);

	// Returns the anti-aliasing method 
	UFUNCTION(BlueprintCallable, Category = Settings)
	uint8 GetAntiAliasingMethod() const;




	UFUNCTION(BlueprintPure, Category = Settings)
	bool IsAntiAliasingMethodDirty() const;

	//UFUNCTION(BlueprintCallable, Category = Settings)
	virtual void LoadSettings(bool bForceReload = false) override;


protected:
	UPROPERTY(config)
	uint8 AntiAliasingMethod;

private:
	UFUNCTION()
	void SetAACommand(uint8 val);
};
