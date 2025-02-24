// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRHandBase.generated.h"



UCLASS()
class LEEMETAXRM_API ALeeXRHandBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRHandBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable,BlueprintPure ,Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	USkeletalMeshComponent* GetHandMesh() { return this->HandMesh.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeVR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance() { return this->HandMesh->GetAnimInstance(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	virtual void OnConstruction(const FTransform& Transform) override;


#pragma region Components

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UMotionControllerComponent> MotionController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class UWidgetInteractionComponent> WidgetInteraction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> HandMesh;

	/**Collision Sphere**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components")
	TObjectPtr<class USphereComponent> GrabSphere;
#pragma endregion

#pragma region HandData

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	EControllerHand HandType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings")
	bool bMirrorAnimation = false;

#pragma endregion
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
