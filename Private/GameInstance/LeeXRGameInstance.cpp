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

void ULeeXRGameInstance::SetNextLevel()
{

	LEE_SCOPE_CYCLE_COUNTER(LeeXRGrabable);

	switch (ActionType)
	{
	case EICTUActionType::EICTU_ActionOne:
		ActionType = EICTUActionType::EICTU_ActionTwo;
		break;
	case EICTUActionType::EICTU_ActionTwo:
		ActionType = EICTUActionType::EICTU_ActionThree;
		break;
	case EICTUActionType::EICTU_ActionThree:
		ActionType = EICTUActionType::EICTU_ActionFour;
		break;
	case EICTUActionType::EICTU_ActionFour:
		ActionType = EICTUActionType::EICTU_ActionFive;
		break;
	}

}

void ULeeXRGameInstance::Init()
{
	Super::Init();

	if (GConfig)
	{
#if UE_BUILD_SHIPPING || UE_BUILD_DEVELOPMENT || UE_BUILD_TEST || UE_BUILD_DEBUG
		GConfig->SetBool(
			TEXT("/Script/EngineSettings.GeneralProjectSettings"),
			TEXT("bStartInVR"),
			true,
			GGameIni
		);
#endif
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
