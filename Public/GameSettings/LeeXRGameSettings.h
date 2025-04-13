// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LeeXRGameSettings.generated.h"


UENUM(BlueprintType)
enum class ELeeXRPlatformBuild : uint8
{
	LEEXR_WindowsVR UMETA(DisplayName = "PC Virtual Reality"),
	LEEXR_Windows UMETA(DisplayName = "Windows"),
	LEEXR_Android UMETA(DisplayName = "VR"),
};


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRGameSettings, Log, All);
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Platform Build"))
	ELeeXRPlatformBuild PlatformBuild;

	UPROPERTY(config)
	float PoolSize = 2800.0f;

	UPROPERTY(config, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float ScreenPercentage = 67.0f;

	UFUNCTION()
	void EditGameSettings(const FString inCommand, float inValue);

protected:
	UPROPERTY(config)
	uint8 AntiAliasingMethod;


};
