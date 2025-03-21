


#include "Widgets/LeeXRHomeMenuWG.h"

#include "Widgets/LeeXRButtonWG.h"
#include "LeeXRUltils.h"
#include "Kismet/KismetStringTableLibrary.h"
#include "LeeXRWidgetUltils.h"

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
}

void ULeeXRHomeMenuWG::NativePreConstruct()
{
	Super::NativePreConstruct();
}

#include "Blueprint/WidgetTree.h"

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
                Buttons[i]->Label->SetText(DisplayName);

				LEE_LOG(LogLeeXRHomeMenuWG, Log, "Display Name %s", *DisplayName.ToString());
            }
        }
    }
}


#undef LOCTEXT_NAMESPACE