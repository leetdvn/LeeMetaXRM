// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeXRGrabbableActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabActors.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLeeGrabActors, Log, All)




class UAbilitySystemComponent;


UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRGrabActors : public ALeeXRGrabbableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRGrabActors();


	UFUNCTION(BlueprintCallable)
	FString ObjectTag() const { return TagObject.ToString(); }

	UFUNCTION(BlueprintCallable)
	bool IsSimulation() { return ActorMesh->IsSimulatingPhysics(); }

	UFUNCTION(BlueprintCallable)
	void SetSimulation(bool bSimulate) { ActorMesh->SetSimulatePhysics(bSimulate); }

	UStaticMeshComponent* GetActorMesh() { return ActorMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class UStaticMeshComponent> ActorMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class UBoxComponent> GrabRegion;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true", DisplayName = "AbilitySystem"))
	UAbilitySystemComponent* AbilityComponent;


	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;

	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

	virtual void InitSettings() override;

	virtual void OnConstruction(const FTransform& Transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);


};
