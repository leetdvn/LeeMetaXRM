// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include <GameplayTagContainer.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabbableActor.h"
#include "LeeXRPhysicsGrabableObject.generated.h"

UCLASS()
class LEEMETAXRM_API ALeeXRPhysicsGrabableObject : public ALeeXRGrabbableActor
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<class UStaticMeshComponent> ShapePhysicsMesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<class UStaticMeshComponent> ShapeCollisionMesh;

public:	
	// Sets default values for this actor's properties
	ALeeXRPhysicsGrabableObject();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FString MaterialParamName;

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FColor StartColor;

	UFUNCTION(Exec, BlueprintCallable, Category = "LeeXR|Func")
	void ConstructionEditor();

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	TObjectPtr<UMaterialInstanceDynamic> MaterialIns;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnGrabObjects(UMotionControllerComponent* inComponent) override;

	virtual void OnReleaseObjects(UMotionControllerComponent* inComponent) override;

	virtual void InitSettings() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
