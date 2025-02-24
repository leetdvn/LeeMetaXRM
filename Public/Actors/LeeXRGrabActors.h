// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/LeeXRInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabActors.generated.h"

UENUM(BlueprintType)
enum class EGrabType : uint8
{
	EGT_Free UMETA(DisplayName = "Free"),
	EGT_Snap UMETA(DisplayName = "Snap"),
	EGT_None UMETA(DisplayName = "None")
};

UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRGrabActors : public AActor, public ILeeXRInteraction
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	bool bIsheld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	TObjectPtr<class UStaticMeshComponent> ActorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	TObjectPtr<class UBoxComponent> GrabRegion;



	virtual void OnGrab(USkeletalMeshComponent* inComponent, FVector& InGrabLocation) override;

	virtual void OnRelease(USkeletalMeshComponent* inComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TObjectPtr<USkeletalMeshComponent> GrabBodySkeletal;
};
