// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRMenuActor.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogLeeXRMenuActor, Log, All);


class UWidgetInteractionComponent;
class UMotionControllerComponent;

UCLASS()
class LEEMETAXRM_API ALeeXRMenuActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MenuActionContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MenuAction_Left;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MenuAction_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MenuCursor_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_MenuCursor_Left;

public:	
	// Sets default values for this actor's properties
	ALeeXRMenuActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorLocationLimitY = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorLocationLimitZ = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorSpeed = -8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float MenuDistanceToWardsCamera =10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	FVector  MenuOffset = FVector(0.f, 0.f, 0.f);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnActionMenu(UWidgetInteractionComponent* inComponent, ETriggerEvent inEvent);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnInputActiveMenu(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnMenuCursorActiveMenu(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetActiveMenu(bool isActive) { isActiveMenu = isActive; }

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	bool IsActive() const { return isActiveMenu; }

	UWidgetInteractionComponent* FindWidgetInteractionReference(EControllerHand inType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UWidgetComponent> WGComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class USceneComponent> OriginComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UStaticMeshComponent> Cursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Components")
	TObjectPtr<class UNiagaraComponent> MenuLaser;

	bool isActiveMenu;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupActorInputComponent();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	UWidgetInteractionComponent* GetWidgetRef(bool isLeft) const { return isLeft ? WGInteractionRefLeft : WGInteractionRefRight; }

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	UMotionControllerComponent* FindMotionControllerReference(bool isLeftHand = true);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void SetReference();


	UFUNCTION(BlueprintCallable)
	void OnMoveComfortableLocation();

	UFUNCTION(BlueprintCallable)
	void UpdateWidgetLocation();

	/// <summary>
	/// Update Cursor Location
	/// </summary>
	/// <param name="input"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateCursorLocation(FVector2D input);

	void LaserPointerInput(UWidgetInteractionComponent* inWidgetAction);

private:

	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefLeft=nullptr;

	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefRight=nullptr;

	TObjectPtr<class UMotionControllerComponent> MotionControllerRef;

	TObjectPtr<class ALeeXRCharacter> XRCharacter = nullptr;

	bool ActiveMenuRight = false;
};
