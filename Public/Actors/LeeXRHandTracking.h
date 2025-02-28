// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/LeeXRHandBase.h"
#include "LeeXRHandTracking.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHandTracking, Log, All);

/**
 * 
 */
UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRHandTracking : public ALeeXRHandBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereThumb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereIndex;


public:
	ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	virtual void GraspObject() override;

	virtual void GraspRelease() override;

	DECLARE_DELEGATE(FCanGraspObject);
	FCanGraspObject OnCanGraspObject;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void InittializeSetup() override;

	UFUNCTION()
	void OnComponentIndexHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, 
		const FHitResult& Hit);

	UFUNCTION()
	void OnComponentThumbHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
private:

	AActor* HitIndexActor=nullptr;
	AActor* HitThumbActor = nullptr;

	void TrackingGrasp();

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
};
