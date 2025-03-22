#pragma once

#include <GameFrameWork/Actor.h>
#include <Async/AsyncWork.h>
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Definitions.h"
#include <EnhancedInputSubsystems.h>
#include "InputMappingContext.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OculusUtilsLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>


UENUM(BlueprintType)
enum class ELeeTickUntilInputPin : uint8
{
	Start,
	Break
};


class FTickUntilAction : public FPendingLatentAction
{
public:
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	bool bIsComplete = false;

	FTickUntilAction(FLatentActionInfo const& LatentInfo) :
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		if (bIsComplete)
		{
			Response.DoneIf(true);
		}
		else
		{
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		}
	}
};

namespace LeeXRUltils
{

	template<class T>
	inline T LeeXRGetEnumValueByString(const FString& enumStr) {
		int32 Index = StaticEnum<T>()->GetValueByName(*enumStr);
		T result = static_cast<T>(Index);
		return result;
	}

	template<class T>
	inline T* LeeXRSPawnActorBP(UObject* inContextObject, const FString inPath, FVector Location=FVector::ZeroVector, FRotator Rotation= FRotator::ZeroRotator) {
		
		UWorld* ContextObject = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull);
		UClass* mClass = StaticLoadClass(T::StaticClass(), inContextObject,*inPath);

		if (ContextObject) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Cast<ACharacter>(inContextObject);
			T* Actor = ContextObject->SpawnActor<T>(mClass, Location, Rotation,SpawnParams);
			return Actor;	
		}
		return nullptr;
	}

	template<class T>
	FORCEINLINE T* LeeXRSPawnActorBP(UObject* inContextObject, TSubclassOf<T> inClass, FVector Location = FVector::ZeroVector, FRotator Rotation = FRotator::ZeroRotator) 
	{
		UWorld* ContextObject = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull);

		if (ContextObject) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = (AActor*)inContextObject;
			T* Actor = ContextObject->SpawnActor<T>(inClass, Location, Rotation, SpawnParams);


			return Actor;
		}
		return nullptr;
	}

	template<class T>
	FORCEINLINE T* LeeXRGetBaseClass(UObject* inObject) {
		return Cast<T>(inObject);
	}

	FORCEINLINE void LeeXRInitMappingContext(const UObject* inContextObject,const UInputMappingContext* inContext,int32 inPriority=0) {

		UWorld* ContextObject = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (APlayerController* PlayerController = ContextObject->GetFirstPlayerController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(inContext, inPriority);
			}
		}
	}

	FORCEINLINE APlayerController* LeeXRGetPlayerController(const UObject* inContextObject) {
		UWorld* ContextObject = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (ContextObject) {
			return ContextObject->GetFirstPlayerController();
		}
		return nullptr;
	}


	template<typename T>
	FORCEINLINE T* LeeXRGetCustomCharacter(const UObject* inContextObject) {
		UWorld* ContextObject = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull);
		if (ContextObject) {
			return Cast<T>(ContextObject->GetFirstPlayerController()->GetPawn());
		}
		return nullptr;
	}

	/// <summary>
	/// Initializer mapping context
	/// </summary>
	/// <param name="inWorld"></param>
	/// <param name="inContext"></param>
	/// <param name="Priority"></param>
	FORCEINLINE void InitializationContext(UWorld* inWorld,UInputMappingContext* inContext,int32 Priority=0) {

		if (!inWorld) return;
		/// Implement the Context
		APlayerController* PlayerController = inWorld->GetFirstPlayerController();
		if(PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(inContext, Priority);
			}
		}
	}

	/// <summary>
	/// Set Material Instance Static Switch Parameter Value
	/// </summary>
	/// <param name="Instance"></param>
	/// <param name="ParameterName"></param>
	/// <param name="Value"></param>
	FORCEINLINE	void SetMaterialInstanceStaticSwitchParameterValue(const UMaterialInstance* Instance, FName ParameterName, bool Value)
	{
		FStaticParameterSet StaticParameters = Instance->GetStaticParameters();
		for (auto& SwitchParameter : StaticParameters.StaticSwitchParameters)
		{
			if (SwitchParameter.ParameterInfo.Name == ParameterName)
			{
				SwitchParameter.Value = Value;
				break;;
			}
		}
		//Instance->UpdateStaticPermutation(StaticParameters);
	}

	/// <summary>
	/// Set Material Instance Dynamic Static Switch Parameter Value
	/// </summary>
	/// <param name="Instance"></param>
	/// <param name="ParameterName"></param>
	/// <param name="Value"></param>
	FORCEINLINE	void SetMaterialInstanceStaticSwitchParameterValue(const UMaterialInstanceDynamic* Instance, FName ParameterName, bool Value)
	{
		FStaticParameterSet StaticParameters = Instance->GetStaticParameters();
		for (auto& SwitchParameter : StaticParameters.StaticSwitchParameters)
		{
			if (SwitchParameter.ParameterInfo.Name == ParameterName)
			{
				SwitchParameter.Value = Value;
				break;;
			}
		}
		//Instance->UpdateStaticPermutation(StaticParameters);
	}

	/// <summary>
	/// Set Material Instance Scalar Parameter Value
	/// </summary>
	/// <param name="Instance"></param>
	/// <param name="ParameterName"></param>
	/// <param name="Value"></param>
	FORCEINLINE	void SetMaterialInstanceVectorParameterValue(UMaterialInstance* Instance, FName ParameterName, FColor Value)
	{
		TArray<FVectorParameterValue> StaticParameters = Instance->VectorParameterValues;
		for (auto& VParameter : StaticParameters)
		{
			if (VParameter.ParameterInfo.Name == ParameterName)
			{
				VParameter.ParameterValue = Value;
				break;;
			}
		}
		//Instance->UpdateOverridableBaseProperties();
	}

	/// <summary>
	/// Set Material Instance Scalar Parameter Value
	/// </summary>
	/// <param name="Instance"></param>
	/// <param name="ParameterName"></param>
	/// <param name="Value"></param>
	FORCEINLINE	void SetMaterialInstanceVectorParameterValue(UMaterialInstance* Instance, FName ParameterName, FLinearColor Value)
	{
		TArray<FVectorParameterValue> StaticParameters = Instance->VectorParameterValues;
		for (auto& VParameter : StaticParameters)
		{
			if (VParameter.ParameterInfo.Name == ParameterName)
			{
				VParameter.ParameterValue = Value;
				break;;
			}
		}
		Instance->UpdateOverridableBaseProperties();
	}

	//Dont Know if this is the right way to do it
	static void TickUntil(const UObject* WorldContextObject, ELeeTickUntilInputPin InputPin, struct FLatentActionInfo LatentInfo)
	{
		if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			auto& LatentActionManager = World->GetLatentActionManager();
			auto Action = LatentActionManager.FindExistingAction<FTickUntilAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
			if (InputPin == ELeeTickUntilInputPin::Start)
			{
				if (Action == nullptr)
				{
					LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FTickUntilAction(LatentInfo));
				}
			}
			else if (InputPin == ELeeTickUntilInputPin::Break)
			{
				if (Action != nullptr)
				{
					Action->bIsComplete = true;
				}
			}
		}
	}

	FORCEINLINE FVector LeeXRGetWorldLocation(const USceneComponent* inComponent) {
		return inComponent->GetComponentToWorld().GetLocation();
	}

	FORCEINLINE void LeeXRPlayerHapicEffect(const UObject* inContextObject, UHapticFeedbackEffect_Base* inEffect, EControllerHand inHand) {
		if (APlayerController* PlayerController = LeeXRGetPlayerController(inContextObject))
		{
			PlayerController->PlayHapticEffect(inEffect, inHand);
		}
	}


	template<typename T>
	FORCEINLINE void LookAtComponent(const UObject* inContextObject,T* TargetComponent, bool isYawOnly)
	{
		/// <summary>
		/// Look at the target component
		if (auto World = GEngine->GetWorldFromContextObject(inContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			if (TargetComponent == nullptr) return;

			//// Get the world location of the display component
			FVector DisplayLocation = TargetComponent->GetComponentLocation();

			// Get the world location of the player's camera
			FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(World, 0)->GetCameraLocation();

			// Calculate the rotation needed to look at the camera
			FRotator FLookAtRot = UKismetMathLibrary::FindLookAtRotation(DisplayLocation, CameraLocation);

			FRotator LookAtRotation = isYawOnly == false ?
				FLookAtRot :
				FRotator(0, FLookAtRot.Yaw, 0);

			// Set the world rotation of the display component to the calculated rotation
			TargetComponent->SetWorldRotation(LookAtRotation);
		}
	}

	FORCEINLINE void LeeXRSetGameConfigStr(const FString ConfigName,const FString ClassName,const FString inValue) {
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);

			LEE_LOG(LeeXRMacro, Log, "Set Config %s", *Section);

			GConfig->SetString(
				*Section,
				*ConfigName,
				*inValue,
				GGameIni
			);
		}
	}

	FORCEINLINE void LeeXRSetGameConfigInt(const FString ConfigName, const FString ClassName, int32 inValue) {
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);
			LEE_LOG(LeeXRMacro, Log, "Set Config %s", *Section);
			GConfig->SetInt(
				*Section,
				*ConfigName,
				inValue,
				GGameIni
			);
		}
	}
	
	FORCEINLINE void LeeXRSetGameConfigBool(const FString ConfigName, const FString ClassName, bool inValue) {
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);
			LEE_LOG(LeeXRMacro, Log, "Set Config %s", *Section);
			GConfig->SetBool(
				*Section,
				*ConfigName,
				inValue,
				GGameIni
			);
		}
	}

	FORCEINLINE int LeeXRGetGameConfigInt(const FString ConfigName, const FString ClassName) {
		int32 Value = 0;
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);
			LEE_LOG(LeeXRMacro, Log, "Get Config %s", *Section);
			GConfig->GetInt(
				*Section,
				*ConfigName,
				Value,
				GGameIni
			);
		}
		return Value;
	}

	FORCEINLINE bool LeeXRGetGameConfigBool(const FString ConfigName, const FString ClassName) {
		bool Value = false;
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);
			LEE_LOG(LeeXRMacro, Log, "Get Config %s", *Section);
			GConfig->GetBool(
				*Section,
				*ConfigName,
				Value,
				GGameIni
			);
		}
		return Value;
	}

	FORCEINLINE FString LeeXRGetGameConfigStr(const FString ConfigName, const FString ClassName) {
		FString Value = "";
		if (GConfig)
		{
			FString Section = FString::Printf(TEXT("/Script/LeeMetaXRM.%s"), *ClassName);
			LEE_LOG(LeeXRMacro, Log, "Get Config %s", *Section);
			GConfig->GetString(
				*Section,
				*ConfigName,
				Value,
				GGameIni
			);
		}
		return Value;
	}
}
