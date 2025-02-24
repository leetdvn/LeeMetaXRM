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
class UWidgetComponent;
class ULeeXRSphereComponent;
class ULeeXRGrabComponent;
class UNiagaraComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* HandMappingContext;

	/*HeadMountedDisiplay And Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> HeadMountedDisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> XRDeviceLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> XRDeviceRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> LeftGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> RightGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOculusXRHandComponent> HandTrackRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UOculusXRHandComponent> HandTrackLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> LeftAim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionControllerComponent> RightAim;

	/**Collision Sphere**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRSphereComponent> IndexLeftCollison;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRSphereComponent> ThumbLeftCollison;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRSphereComponent> IndexRightCollison;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRSphereComponent> ThumbRightCollison;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetInteractionLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> WidgetInteractionRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRGrabComponent> HeldComponentLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULeeXRGrabComponent> HeldComponentRight;

	/*Niagara*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Actors", meta = (AllowPrivateAccess="true"))
	TSubclassOf<AActor> ActorToSpawn;
	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_GrabLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_GrabRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_LMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LeeVR Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_RMenuToogle;


public:
	// Sets default values for this pawn's properties
	ALeeXRPawn();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void StartTeleportTrace();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void TeleportTrace(FVector StartPos,FVector ForwardVec);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void EndTeleportTrace();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void TryTeleport();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMove(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMoveStart(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAMoveComplete(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IATurnStart();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IATurnCompleted();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabLeftStart();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabLeftCompleted();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabRightStart();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IAGrabRightCompleted();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IALMenuToogle(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void IARMenuToogle();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	ULeeXRGrabComponent* GetGrapComponentNearController(UMotionControllerComponent* MotionController);

	UFUNCTION()
	void OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

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

	bool IsValidTeleportLocation(FHitResult Hit,FVector &ProjectedLocation);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	void ToogleMenu(bool isLeft);

	void InitializeComponents();
	
	void InitializeMappingContext();

	FVector bStartLine;
	FVector bEndLine;
};
