// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actors/LeeXRHandBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeeXRCharacter.generated.h"



DEFINE_LOG_CATEGORY_STATIC(LogLeeXRCharacter,Log,All)

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ALeeXRCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance(bool isLeft);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	ELeeXRHandType HandType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class USceneComponent> XROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class ALeeXRHandBase> XRHandLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class ALeeXRHandBase> XRHandRight;

#pragma endregion Components

#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> HandMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_GraspLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_GraspRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_LMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_RMenuToogle;
#pragma endregion Input

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Test Moving IA Move
	/// </summary>
	UFUNCTION()
	void OnMoving();


private:

	/// <summary>
	/// Initialize Context
	/// </summary>
	void InitContext();

	UFUNCTION()
	void OnHandTrigger(const FInputActionInstance& ActionInstance);
	/// <summary>
	/// Input Action for Grabing
	/// </summary>
	/// <param name="ActionInstance"></param>
	UFUNCTION()
	void OnHandGrabing(const FInputActionInstance& ActionInstance);

	/// <summary>
	/// Input Action for Releasing
	/// </summary>
	/// <param name="ActionInstance"></param>
	UFUNCTION()
	void OnHandRelease(const FInputActionInstance& ActionInstance);

	/// <summary>
	/// Hand Initialize
	/// </summary>
	void HandInitialize();

	void HandTrackingInitialize();
};
