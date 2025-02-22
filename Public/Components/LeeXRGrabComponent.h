// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Definitions.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LeeXRGrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEEMETAXRM_API ULeeXRGrabComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeeXRGrabComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
