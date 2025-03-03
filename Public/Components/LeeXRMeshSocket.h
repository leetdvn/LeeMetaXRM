// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LeeXRMeshSocket.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRMeshSocket : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	ULeeXRMeshSocket(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category ="LeeVR Settings|Properties")
	FString TargetSocketName;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	FString MaterialParamName;

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeVR Settings|Properties", meta = (DefaultValue ="FLinearColor::Green"))
	FColor StartColor;

	UFUNCTION(Exec, BlueprintCallable, Category = "LeeXR|Func")
	void ConstructionEditor();

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	TObjectPtr<UMaterialInstanceDynamic> MaterialIns;

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeVR Settings|Properties")
	float DefaultParamValue;

protected:
	virtual void BeginPlay() override;



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

private:
};
