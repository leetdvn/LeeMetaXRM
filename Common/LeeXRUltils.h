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
	FORCEINLINE	void SetMaterialInstanceStaticSwitchParameterValue(UMaterialInstance* Instance, FName ParameterName, bool Value)
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
		Instance->UpdateStaticPermutation(StaticParameters);
	}

	/// <summary>
	/// Set Material Instance Dynamic Static Switch Parameter Value
	/// </summary>
	/// <param name="Instance"></param>
	/// <param name="ParameterName"></param>
	/// <param name="Value"></param>
	FORCEINLINE	void SetMaterialInstanceStaticSwitchParameterValue(UMaterialInstanceDynamic* Instance, FName ParameterName, bool Value)
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
		Instance->UpdateStaticPermutation(StaticParameters);
	}

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
}
