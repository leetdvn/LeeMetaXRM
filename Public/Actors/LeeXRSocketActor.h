// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRSocketActor.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRSocketActor, Log, All);


UCLASS()
class LEEMETAXRM_API ALeeXRSocketActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletonBasic;

public:	
	// Sets default values for this actor's properties
	ALeeXRSocketActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (AllowPrivateAccess="true",DefaultValue="FColor::Green"))
	FColor PreviewColor=FColor::Green;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
