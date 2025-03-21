


#include "Widgets/LeeXRButtonWG.h"

#include "Components/SizeBox.h"

#include "LeeXRUltils.h"

using namespace LeeXRUltils;

#define LOCTEXT_NAMESPACE "LeeMetaXRModules"

ULeeXRButtonWG::ULeeXRButtonWG(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULeeXRButtonWG::NativeConstruct()
{
	Super::NativeConstruct();

    if (Label && LeeSizeBox)
    {
        FVector2D DesiredSize = Label->GetDesiredSize();
        LeeSizeBox->SetWidthOverride(DesiredSize.X);
        LeeSizeBox->SetHeightOverride(DesiredSize.Y);

		LEE_LOG(LogTemp, Log, "Desired Size %s", *DesiredSize.ToString());
    }
}


#undef LOCTEXT_NAMESPACE