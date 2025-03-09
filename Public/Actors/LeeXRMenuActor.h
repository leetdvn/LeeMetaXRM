// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRMenuActor.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLeeXRMenuActor, Log, All);

UCLASS()
class LEEMETAXRM_API ALeeXRMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRMenuActor();

	UWidgetInteractionComponent* FindWidgetInteractionReference(EControllerHand inType);

	UWidgetInteractionComponent* GetWidgetRef(bool isLeft) const { return isLeft ? WGInteractionRefLeft : WGInteractionRefRight; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefLeft=nullptr;
	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefRight=nullptr;

	TObjectPtr<class UMOtionControllerComponent> MotionControllerRef;
};
