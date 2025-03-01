// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Definitions.h"
#include "DataAssets/LeeXRHandDataAsset.h"
#include "Actors/LeeXRHandBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LeeXRCharacter.generated.h"



DEFINE_LOG_CATEGORY_STATIC(LogLeeXRCharacter,Log,All)

DECLARE_MEMORY_STAT_EXTERN(TEXT("ICTUCharacter"), STAT_ICTUCharacter, STATGROUP_ICTUMV, );
DECLARE_DWORD_COUNTER_STAT_EXTERN(TEXT("ICTUCharacterMemory"), STAT_ICTUCharacterMemory, STATGROUP_ICTUMV, );


/**
 * XR Character Class
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class LEEMETAXRM_API ALeeXRCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Actors", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Actors", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Actors", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TeleportVisualizer;

public:
	// Sets default values for this character's properties
	ALeeXRCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LeeXR|Func", meta = (BlueprintThreadSafe))
	UAnimInstance* GetHandAnimInstance(bool isLeft);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void TeleportTrace(FVector StartPos, FVector ForwardVec);

	bool IsValidTeleportLocation(FHitResult Hit, FVector& ProjectedLocation);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Varibles")
	TArray<FVector> TeleportTracePathPositions;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Varibles")
	FVector TeleportProjectPointToNavigationQueryExtent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Varibles")
	FVector ProjectedTeleportLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Varibles")
	bool bValidTeleportLocation = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Varibles")
	bool bTeleportTraceActive;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma region Components

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	TObjectPtr<UStaticMeshComponent> DisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	ELeeXRHandType HandType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	TObjectPtr<class USceneComponent> XROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	TObjectPtr<ALeeXRHandBase> XRHandLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Components")
	TObjectPtr<ALeeXRHandBase> XRHandRight;

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "ALeeVR Settings|Data")
	ULeeXRHandDataAsset* DataLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALeeVR Settings|Data")
	ULeeXRHandDataAsset* DataRight;

#pragma endregion Components

#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputMappingContext> HandMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_LMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_RMenuToogle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALeeVR Settings|Input")
	TObjectPtr<class UInputAction> IA_Grasp;


#pragma endregion Input

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// Test Moving IA Move
	/// </summary>
	UFUNCTION()
	void OnMoving();

	UFUNCTION()
	void OnHandGrabing(const FInputActionInstance& ActionInstance);
private:


	/// <summary>
	/// Hand Initialize
	/// </summary>
	ALeeXRHandBase* HandInitialize(ELeeXRHandType inType,bool isLeft);

	template<typename T>
	ALeeXRHandBase* InitializeHandActor(const FString inLeftHandPath);

	template<typename T>
	ALeeXRHandBase* InitializeHandActor(TSubclassOf<T> inClass);

};

template<typename T>
inline ALeeXRHandBase* ALeeXRCharacter::InitializeHandActor(const FString inHandPath)
{
	if (inHandPath.IsEmpty()) return nullptr;
	// Path Actor Blueprint
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	ALeeXRHandBase* HandBase = LeeXRSPawnActorBP<T>(this, inHandPath);
	if (HandBase == nullptr) return nullptr;
	HandBase->AttachToComponent(XROrigin, AttachRules);

	return nullptr;
}

template<typename T>
inline ALeeXRHandBase* ALeeXRCharacter::InitializeHandActor(TSubclassOf<T> inClass)
{
	if (inClass == nullptr) return nullptr;
	// Path Actor Blueprint

	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	// Spawn the hands
	ALeeXRHandBase* HandBase = LeeXRSPawnActorBP<T>(this,inClass);
	if (HandBase == nullptr) return nullptr;
	HandBase->AttachToComponent(XROrigin, AttachRules);

	return HandBase;
}
