// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeeXRCharacter.generated.h"



UCLASS()
class LEEMETAXRM_API ALeeXRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALeeXRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings | Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings | Components")
	TObjectPtr<class USceneComponent> XROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnMoving();

private:
	void InitContext();
};
