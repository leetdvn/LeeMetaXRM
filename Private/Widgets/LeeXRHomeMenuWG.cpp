


#include "Widgets/LeeXRHomeMenuWG.h"

#include "LeeXRUltils.h"
#include "Kismet/KismetStringTableLibrary.h"
#include "LeeXRWidgetUltils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include <Interfaces/LeeXRInterfaceHMD.h>
#include "Blueprint/WidgetTree.h"
#include "Actors/LeeXRMenuActor.h"
#include "APawn/LeeXRCharacter.h"
#include "GameInstance/LeeXRGameInstance.h"

using namespace LeeXRUltils;
using namespace LeeXRWidgetUltils;

#define LOCTEXT_NAMESPACE "LeeMetaXRModules"

ULeeXRHomeMenuWG::ULeeXRHomeMenuWG(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Constructor code here

}

void ULeeXRHomeMenuWG::SetFreeLabel(FText inText)
{
	if (inText.IsEmpty()) return;

	Free->Label->SetText(inText);
}

void ULeeXRHomeMenuWG::SetTourLabel(FText inText)
{
	if (inText.IsEmpty()) return;

	Tourist->Label->SetText(inText);
}

void ULeeXRHomeMenuWG::SetTrainLabel(FText inText)
{
	if (inText.IsEmpty()) return;
	Train->Label->SetText(inText);
}

void ULeeXRHomeMenuWG::NativeConstruct()
{
	Super::NativeConstruct();

	InitBindMenu();
}

void ULeeXRHomeMenuWG::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULeeXRHomeMenuWG::OnHovered()
{
	//LeeScreenLog("Hovered", FColor::Green);
}

void ULeeXRHomeMenuWG::OnFreeClicked()
{
	ChangeMap(Free);
}

void ULeeXRHomeMenuWG::OnTouristClicked()
{
	ChangeMap(Tourist);
}

void ULeeXRHomeMenuWG::OnTrainClicked()
{
	ChangeMap(Train);
	if (auto HMD = TScriptInterface<ILeeXRInterfaceHMD>())
	{
		//HMD->OnHMDLevelChanged(NewMap);
		//UGameplayStatics::OpenLevel(this, FName(*NewMap));
		LeeScreenLog("Train Clicked",FColor::Blue);
	}
}

void ULeeXRHomeMenuWG::OnExitClicked()
{
	return UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void ULeeXRHomeMenuWG::ChangeMap(ULeeXRButtonWG* inButton)
{
	if (inButton->MapName.IsEmpty()) return;

	FString NewMap = inButton->MapName;

	FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this);

	if (NewMap == CurrentMap) return;

	if (auto XRCharacter = LeeXRGetCustomCharacter<ALeeXRCharacter>(this))
	{
		if (auto HMD = TScriptInterface<ILeeXRInterfaceHMD>(XRCharacter))
		{
			if (auto GamsIns = GetGameInstance<ULeeXRGameInstance>())
			{
				//XRCharacter->SetLockHMD(GamsIns->bIsLockHMD);
				XRCharacter->NextLevel = 3;
			}

			HMD->Execute_OnHMDLevelChanged(XRCharacter, NewMap);
			UGameplayStatics::OpenLevel(this, FName(*NewMap));
			LEE_LOG(LogLeeXRHomeMenuWG, Log, "Changed %s", *NewMap);
		}
	}
}

void ULeeXRHomeMenuWG::InitBindMenu()
{

	Free->LeeBtn->OnHovered.AddDynamic(this, &ULeeXRHomeMenuWG::OnHovered);

	if (Free->LeeBtn->OnClicked.IsBound()) Free->LeeBtn->OnClicked.Clear();
	Free->LeeBtn->OnClicked.AddDynamic(this, &ULeeXRHomeMenuWG::OnFreeClicked);

	if (Tourist->LeeBtn->OnClicked.IsBound()) Tourist->LeeBtn->OnClicked.Clear();
	Tourist->LeeBtn->OnClicked.AddDynamic(this, &ULeeXRHomeMenuWG::OnTouristClicked);

	if (Train->LeeBtn->OnClicked.IsBound()) Train->LeeBtn->OnClicked.Clear();
	Train->LeeBtn->OnClicked.AddDynamic(this, &ULeeXRHomeMenuWG::OnTrainClicked);

	if (Exit->LeeBtn->OnClicked.IsBound()) Exit->LeeBtn->OnClicked.Clear();
	Exit->LeeBtn->OnClicked.AddDynamic(this, &ULeeXRHomeMenuWG::OnExitClicked);

}

void ULeeXRHomeMenuWG::LoadDisplayNames(UStringTable* inTable)
{
    if (IsValid(inTable))
    {
    
        FName TableId = inTable->GetStringTableId();

		// Get all buttons
		//Rule Button Name must match the key name
		//Rule 1: Find all buttons
		TArray<ULeeXRButtonWG*> Buttons = LeeXRFindWidgetsOfClass<ULeeXRButtonWG>(this);

		//Rule 2: Find all keys from the string table inTable
        TArray<FString> Keys = UKismetStringTableLibrary::GetKeysFromStringTable(TableId);

		///Rule 3: Set the display name of each button name to  the Keys
        for (int32 i = 0; i < Buttons.Num(); ++i)
        {
            if (Buttons[i]->GetName() == Keys[i])
            {
                FText DisplayName = FText::FromStringTable(TableId, Keys[i]);
                Buttons[i]->DisplayName = DisplayName;
				if(Buttons[i]->Label)
					Buttons[i]->Label->SetText(DisplayName);

				LEE_LOG(LogLeeXRHomeMenuWG, Log, "Display Name %s", *DisplayName.ToString());
            }
        }
    }
}


#undef LOCTEXT_NAMESPACE