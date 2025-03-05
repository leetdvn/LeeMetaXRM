// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeeXRUltils.h"
#include "OculusUtilsLibrary.h"
#include "CoreMinimal.h"
#include "Actors/LeeXRHandBase.h"
#include "OculusXRInputFunctionLibrary.h"
#include "LeeXRHandTracking.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRHandTracking, Log, All);


UENUM(BlueprintType)
enum class LeeHandPose : uint8
{
	LHandMove UMETA(DisplayName = "Teleport"),
	LHandGrasp UMETA(DisplayName = "Grasp"),
	LHandRelease UMETA(DisplayName = "Release"),
	LHandMenu UMETA(DisplayName = "Menu"),
};



/**
 * 
 */
UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRHandTracking : public ALeeXRHandBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereThumb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereIndex;


public:
	ALeeXRHandTracking(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	virtual void GraspObject() override;

	virtual void GraspRelease() override;

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", Keywords = "sleep", ExpandEnumAsExecs = "InputPin"))
	void TickUntilGrasp(const UObject* WorldContextObject, ELeeTickUntilInputPin InputPin, struct FLatentActionInfo LatentInfo);



protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void InittializeSetup() override;

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

	void AttachOculusHandTracking(const EOculusXRBone inEbone, USphereComponent* inSphere);

	bool IsGrabable(TArray<AActor*> inOverlapActors,AActor* InArrayAct);

private:

	void TrackingGrasp();
	AActor* HitIndexActor = nullptr;
	AActor* HitThumbActor = nullptr;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
};
