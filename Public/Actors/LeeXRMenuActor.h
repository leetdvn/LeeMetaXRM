// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
public:	
	// Sets default values for this actor's properties
	ALeeXRMenuActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorLocationLimitY = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorLocationLimitZ = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR Settings|Properties")
	float CursorSpeed = -8.f;

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnActionMenu(UWidgetInteractionComponent* inComponent, ETriggerEvent inEvent);

	UFUNCTION(BlueprintCallable, Category = "LeeXR|Func")
	void OnInputActiveMenu(const FInputActionInstance& ActionInstance);

	UWidgetInteractionComponent* FindWidgetInteractionReference(EControllerHand inType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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

private:

	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefLeft=nullptr;

	TObjectPtr<class UWidgetInteractionComponent> WGInteractionRefRight=nullptr;

	TObjectPtr<class UMotionControllerComponent> MotionControllerRef;
};
