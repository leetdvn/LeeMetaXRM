// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/LeeXRGameSettings.h"
#include "Kismet/GameplayStatics.h"

ULeeXRGameSettings::ULeeXRGameSettings(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//Super::SetToDefaults();	
	SetAACommand((uint8)AntiAliasingMethod);
}

void ULeeXRGameSettings::SetAntiAliasingMethod(EAntiAliasingMethod Value)
{
	uint8 val = (uint8)Value;
	static IConsoleVariable* SetAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
	if (ensure(SetAA))
	{
		// ECVF_SetByGameSetting , ECVF_SetByCode, ECVF_SetByProjectSetting
		SetAA->Set(val, ECVF_SetByGameSetting);
#if WITH_EDITOR
		SetAA->Set(val, ECVF_SetByProjectSetting);
#endif

	}
	SetAACommand(val);
	AntiAliasingMethod = val;
}




void ULeeXRGameSettings::SetAACommand(uint8 val) {

	if (GEngine != nullptr) {

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("SetAACommand %d"), val));

		if (GEngine->GameViewport != nullptr) {
			UWorld* world = GEngine->GameViewport->GetWorld();
			if (world != nullptr) {
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("SET AA Index %d"), val));
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(world, 0)) {
					PC->ConsoleCommand("r.AntiAliasingMethod " + val);
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("PLAYER CONTROLLER IS NULL"));
				}
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("WORLD IS NULL"));
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("GEngine->GameViewport is null"));
		}
	}

}



uint8 ULeeXRGameSettings::GetAntiAliasingMethod() const
{

	return AntiAliasingMethod;
}



bool ULeeXRGameSettings::IsAntiAliasingMethodDirty() const
{
	bool bIsDirty = false;
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->ViewportFrame)
	{
		static IConsoleVariable* GetAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
		int32 aa = (uint8)GetAA->GetInt();
		bIsDirty = (AntiAliasingMethod != aa);
	}
	return bIsDirty;
}

void ULeeXRGameSettings::LoadSettings(bool bForceReload)
{

	Super::LoadSettings(bForceReload);

	SetAACommand((uint8)AntiAliasingMethod);




}
