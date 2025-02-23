// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LeeXRSphereComponent.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRSphereCollision,Log,All)
/**
 * 
 */
UCLASS(ClassGroup = "Collision", editinlinenew, hidecategories = (Object, LOD, Lighting, TextureStreaming), meta = (DisplayName = " Lee Sphere Collision", BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRSphereComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	ULeeXRSphereComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnHitComp(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	virtual void BeginPlay() override;

};
