

#pragma once

#include "Blueprint/WidgetTree.h"
#include "Internationalization/StringTable.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeeXRHomeMenuWG.generated.h"
/**
 * 
 */

DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHomeMenuWG, Log, All);

class UButton;


UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRHomeMenuWG : public UUserWidget
{
	GENERATED_BODY()
public:
	ULeeXRHomeMenuWG(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Free To Play"))
	TObjectPtr<class ULeeXRButtonWG> Free;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Tourist"))
	TObjectPtr<class ULeeXRButtonWG> Tourist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Trainning"))
	TObjectPtr<class ULeeXRButtonWG> Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Exit"))
	TObjectPtr<class ULeeXRButtonWG> Exit;

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetFreeLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetTourLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetTrainLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void LoadDisplayNames(UStringTable* inTable);


	//template<typename T>
	//T* LeeXRFindWidget(const FName& Name) const;


	//template<typename T>
	//TArray<T*> LeeXRFindWidgetsOfClass() const;

protected:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;


private:

};

//template<typename T>
//inline T* ULeeXRHomeMenuWG::LeeXRFindWidget(const FName& Name) const
//{
//	UWidget* LocalParent = GetRootWidget()->GetOuter();
//
//	if (auto TreeWG = Cast<UWidgetTree>(LocalParent))
//	{
//		if (auto Widget = TreeWG->FindWidget<T>(Name))
//		{
//			return Widget;
//		}
//	}
//	return nullptr;
//}
//
//template<typename T>
//inline TArray<T*> ULeeXRHomeMenuWG::LeeXRFindWidgetsOfClass() const
//{
//
//	TArray<UWidget*> Widgets{};
//	TArray<T*> FoundWidgets{};
//
//	UObject* LocalParent = GetRootWidget()->GetOuter();
//
//	if (auto RootWg = Cast<UWidgetTree>(LocalParent))
//	{
//		RootWg->GetAllWidgets(Widgets);
//
//		for (auto Widget : Widgets)
//		{
//			if (auto FoundWidget = Cast<T>(Widget))
//			{
//				FoundWidgets.AddUnique(FoundWidget);
//			}
//		}
//	}
//	return FoundWidgets;
//
//}
