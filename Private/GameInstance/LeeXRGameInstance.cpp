// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/LeeXRGameInstance.h"

#include "Definitions.h"
#include "LeeXRUltils.h"
#include "APawn/LeeXRCharacter.h"


using namespace LeeXRUltils;



ULeeXRGameInstance::ULeeXRGameInstance()
	:Super()
{
}

void ULeeXRGameInstance::Init()
{
	Super::Init();

	if (GConfig)
	{
		CurrentLevel = LeeXRGetGameConfigInt(TEXT("CurrentLevel"), TEXT("ULeeXRGameInstance"));

		int32 Demo = LeeXRGetGameConfigInt(TEXT("ActionType"), TEXT("ULeeXRGameInstance"));
		LEE_LOG(LogLeeICTUGameInstance, Log, "Current Level %d", CurrentLevel);
		LEE_LOG(LogLeeICTUGameInstance, Log, "Current Level %d", Demo);

	}

	
}

void ULeeXRGameInstance::Shutdown()
{
	Super::Shutdown();

	LEE_LOG(LogLeeICTUGameInstance, Log, "ICTU Game ShutDown");

}

void ULeeXRGameInstance::OnStart()
{
	Super::OnStart();

	LEE_LOG(LogLeeICTUGameInstance, Log, "ICTU Game Start");

	//ActionType = EICTUActionType::EICTU_ActionOne;

}
