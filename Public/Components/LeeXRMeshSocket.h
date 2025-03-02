// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LeeXRMeshSocket.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class LEEMETAXRM_API ULeeXRMeshSocket : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	ULeeXRMeshSocket(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category ="LeeVR Settings|Input Properties")
	FString TargetSocketName;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeVR Settings|Input Properties")
	FString MaterialParamName;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient,EditAnyWhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> MaterialIns;

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);
};
