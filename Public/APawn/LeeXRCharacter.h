// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeeXRCharacter.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRCharacter,Log,All)

UCLASS()
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components")
	TObjectPtr<class USceneComponent> XROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> HandMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_GrabLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_GrabRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_LMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_RMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Hand")
	TObjectPtr<class ALeeXRHandBase> XRHandLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Hand")
	TObjectPtr<class ALeeXRHandBase> XRHandRight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnMoving();

	UFUNCTION()
	void OnActionGrab(const FInputActionInstance& ActionInstance);
private:

	//Init Context
	void InitContext();

	/// <summary>
	/// Grab Action 
	/// </summary>
	/// <param name="ActionInstance"></param>
	/// <param name="isLeft"></param>
	void OnGrabObjects(const FInputActionInstance& ActionInstance,bool isLeft);


	void HandInitialize();
};
