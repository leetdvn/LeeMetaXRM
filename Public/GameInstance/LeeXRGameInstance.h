// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "APawn/LeeXRCharacter.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LeeXRGameInstance.generated.h"


UENUM(BlueprintType)
enum class EICTUGameModeType : uint8
{
	EICTU_FactoryFree UMETA(DisplayName = "Factory Free"),
	EICTU_FactoryTour UMETA(DisplayName = "Factory Tour")
};

UENUM(BlueprintType)
enum class EICTUTourState : uint8
{
	EICTU_TourStart UMETA(DisplayName = "Tour Start"),
	EICTU_TourEnd UMETA(DisplayName = "Tour End")
};


DEFINE_LOG_CATEGORY_STATIC(LogLeeICTUGameInstance, Log, All);

/**
 * 
 */
UCLASS()
class LEEMETAXRM_API ULeeXRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULeeXRGameInstance();


protected:

	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void OnStart() override;
};
