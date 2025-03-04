// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <NiagaraComponent.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRTeleportActor.generated.h"




UCLASS(BlueprintType)
class LEEMETAXRM_API ALeeXRTeleportActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Setting|Properties", meta = (AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;
public:	
	// Sets default values for this actor's properties
	ALeeXRTeleportActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Setting|Properties")
	TObjectPtr<UNiagaraComponent> NSTeleportRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Setting|Properties")
	TObjectPtr<UNiagaraComponent> NSPlayAreaBound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
