

#pragma once

#include "Widgets/LeeXRButtonWG.h"
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

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetFreeLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetTourLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetTrainLabel(FText inText);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void LoadDisplayNames(UStringTable* inTable);

	UFUNCTION(BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UButton* GetFreeButton() const { return Free->LeeBtn.Get(); }

	UFUNCTION(BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UButton* GetTouristButton() const { return Tourist->LeeBtn.Get(); }

	UFUNCTION(BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UButton* GetTrainButton() const { return Train->LeeBtn.Get(); }

	UFUNCTION(BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UButton* GetExitButton() const { return Exit->LeeBtn.Get(); }

protected:

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Free To Play"))
	TObjectPtr<class ULeeXRButtonWG> Free;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Tourist"))
	TObjectPtr<class ULeeXRButtonWG> Tourist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Trainning"))
	TObjectPtr<class ULeeXRButtonWG> Train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget, DisplayName = "Exit"))
	TObjectPtr<class ULeeXRButtonWG> Exit;

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnHovered();

	UFUNCTION()
	void OnFreeClicked();

	UFUNCTION()
	void OnTouristClicked();

	UFUNCTION()
	void OnTrainClicked();

	UFUNCTION()
	void OnExitClicked();

private:

	void ChangeMap(ULeeXRButtonWG* inButton);

	void InitBindMenu();

};