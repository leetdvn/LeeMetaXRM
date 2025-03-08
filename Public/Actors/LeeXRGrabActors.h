// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
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
	bool IsTag(const FGameplayTag inObjectTag) const { return ObjectTags == inObjectTag; }

	UFUNCTION(BlueprintCallable)
	FString ObjectTag() const { return ObjectTags.ToString(); }

	UFUNCTION(BlueprintCallable)
	bool IsSimulation() { return ActorMesh->IsSimulatingPhysics(); }

	UFUNCTION(BlueprintCallable)
	void SetSimulation(bool bSimulate) { ActorMesh->SetSimulatePhysics(bSimulate); }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class UStaticMeshComponent> ActorMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	TObjectPtr<class UBoxComponent> GrabRegion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FGameplayTag ObjectTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true", DisplayName = "AbilitySystem"))
	UAbilitySystemComponent* AbilityComponent;

	virtual void OnGrab(UObject* inComponent,const FVector& InGrabLocation) override;

	virtual void OnRelease(UObject* inComponent) override;

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
