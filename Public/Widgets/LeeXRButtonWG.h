

#pragma once

#include "Components/RichTextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeeXRButtonWG.generated.h"


/**
 * 
 */

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRButtonWG : public UUserWidget
{
	GENERATED_BODY()
public:
	ULeeXRButtonWG(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget))
	TObjectPtr<class UButton> LeeBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget))
	TObjectPtr<class URichTextBlock> Label;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (BindWidget))
	TObjectPtr<class USizeBox> LeeSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (ExposeOnSpawn = "true"))
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties", meta = (ExposeOnSpawn = "true"))
	FString MapName;

	UFUNCTION(BlueprintPure, Category = "LeeXR|Func",meta=(BlueprintThreadSafe))
	UButton* GetButton() const { return LeeBtn.Get(); }

protected:
	virtual void NativeConstruct() override;
};
