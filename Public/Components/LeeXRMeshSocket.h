// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LeeXRMeshSocket.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMeshSocketUpdate);

struct FGameplayTag;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ULeeXRMeshSocket : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	ULeeXRMeshSocket(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties",meta=(DefaultValue="OnOff"))
	FString MaterialParamName= TEXT("OnOff");

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties", meta = (DefaultValue ="FLinearColor::Green"))
	FColor StartColor;

	UFUNCTION(Exec, BlueprintCallable, Category = "LeeXR|Func")
	void ConstructionEditor();

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	TObjectPtr<UMaterialInstanceDynamic> MaterialIns;

	UPROPERTY(Transient, EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	float DefaultParamValue;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	FGameplayTag SocketTag;


	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "LeeXR Settings|Properties")
	bool IsDone = false;

	UPROPERTY(EditAnyWhere, BlueprintAssignable,Category = "LeeXR|Delegate", meta = (DisplayName = "On Mesh Snap"))
	FOnMeshSocketUpdate OnMeshSocketUpdate;

	UFUNCTION(BlueprintCallable)
	void SetFlicker(bool isTurnOn);

	UFUNCTION(BlueprintCallable)
	void SetCorrectShape(bool isTrue);
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
