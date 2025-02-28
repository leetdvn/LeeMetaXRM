// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ICTUGrabbableActor.h"
#include "Interfaces/LeeXRInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabActors.generated.h"



UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRGrabActors : public AICTUGrabbableActor, public ILeeXRInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRGrabActors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	EGrabType GrabType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	bool bIsheld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	TObjectPtr<class UBoxComponent> GrabRegion;

	virtual void OnGrab(UObject* inComponent,const FVector& InGrabLocation) override;

	virtual void OnRelease(UObject* inComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UObject* GrabUObject;
};
