// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MotionControllerComponent.h"
#include "Definitions.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LeeXRGrabComponent.generated.h"


UENUM()
enum class ELeeXRGrabType : uint8
{
	LNONE UMETA(DisplayName="None"),
	LFREE UMETA(DIsplayName="Free"),
	LSNAP UMETA(DisplayName = "Snap"),
	LCUSTOM UMETA(DisplayName = "Custom")
};



UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeeXRGrabComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetSholdSimulationOnDrop();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetPrimitiveComPhysics(bool bShouldSimulate);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void AttachParentToMotionController(UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "LeeXR|Func",meta=(BlueprintThreadSafe))
	bool TryGrab(UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	bool TryRelease();

	/// <summary>
	/// Should simulate on drop
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	ELeeXRGrabType GrabType = ELeeXRGrabType::LNONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<UHapticFeedbackEffect_Base> OnGrabHapticEffect;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	bool bIsHeld = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	bool bShouldSimulateOnDrop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<UMotionControllerComponent> MotionControllerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	FRotator PrimitiveGrabRotation;





protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	void UpdateGrabType();
};
