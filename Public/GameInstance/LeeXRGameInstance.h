// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
enum class EICTUActionType : uint8
{
	EICTU_ActionOne UMETA(DisplayName = "Level One"),
	EICTU_ActionTwo UMETA(DisplayName = "Level Two"),
	EICTU_ActionThree UMETA(DisplayName = "Level Three"),
	EICTU_ActionFour UMETA(DisplayName = "Level Four"),
	EICTU_ActionFive UMETA(DisplayName = "Level Five"),

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

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	EICTUActionType GetActionType() const { return ActionType; };

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetActionType(EICTUActionType inType) { ActionType = inType; };

protected:

	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void OnStart() override;


	EICTUActionType ActionType;
};
