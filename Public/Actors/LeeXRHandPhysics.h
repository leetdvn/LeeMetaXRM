// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/LeeXRHandBase.h"
#include "LeeXRHandPhysics.generated.h"

/**
 * 
 */
UCLASS()
class LEEMETAXRM_API ALeeXRHandPhysics : public ALeeXRHandBase
{
	GENERATED_BODY()
	
public:
	ALeeXRHandPhysics(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void InittializeSetup() override;
	bool bIsCanGrasp;

#pragma region Components

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<UStaticMeshComponent> CubeConstraint;

	UStaticMeshComponent* GetCubeConstraint() const { return CubeConstraint; }

	USkeletalMeshComponent* GetHandSkeletal() const { return HandSkeletal; }
#pragma endregion


protected:

};
