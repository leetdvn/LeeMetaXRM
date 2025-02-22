// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MotionControllerComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LeeXRPawn.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LeeXRCharacters, Log, All)

class UMotionControllerComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UVRNotificationsComponent;

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRPawn : public APawn
{
	GENERATED_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* LCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVRNotificationsComponent> Notify;
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Turn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_GrabLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_GrabRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_LMenuToogle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_RMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> LeftGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> RightGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> LeftAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> RightAim;

public:
	// Sets default values for this pawn's properties
	ALeeXRPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void StartTeleportTrace();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMove();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMoveStart();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMoveComplete();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IATurn();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabLeft();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabRight();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IALMenuToogle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	FVector ProjectedTeleportLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	bool bValidTeleportLocation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	bool bTeleportTraceActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	float GrabRadiusFromGribPosition = 6.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	float SnapTurnDegrees = -45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	TArray<FVector> TeleportTracePathPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeeVR Settings|Varibles")
	FVector TeleportProjectPointToNavigationQueryExtent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
