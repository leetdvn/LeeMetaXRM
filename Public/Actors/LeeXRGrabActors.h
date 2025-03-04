// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "LeeXRGrabbableActor.h"
#include "Interfaces/LeeXRInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRGrabActors.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLeeGrabActors, Log, All)


class UAbilitySystemComponent;


UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRGrabActors : public ALeeXRGrabbableActor, public ILeeXRInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRGrabActors();

	UFUNCTION(BlueprintCallable)
	bool IsTag(const FGameplayTag inObjectTag) const { return ObjectTags == inObjectTag; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings")
	EGrabType GrabType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	bool bIsheld;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeXR Settings")
	TObjectPtr<class UBoxComponent> GrabRegion;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "LeeXR Settings")
	FGameplayTag ObjectTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LA Settings", meta = (AllowPrivateAccess = "true", DisplayName = "AbilitySystem"))
	UAbilitySystemComponent* AbilityComponent;

	virtual void OnGrab(UObject* inComponent,const FVector& InGrabLocation) override;

	virtual void OnRelease(UObject* inComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UObject* GrabUObject;

	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);
};
