#pragma once

#include <GameFrameWork/Actor.h>
#include <Async/AsyncWork.h>
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Definitions.h"
#include <EnhancedInputSubsystems.h>
#include "InputMappingContext.h"

namespace LeeXRUltils
{

	template<class T>
	inline T LeeXRGetEnumValueByString(const FString inEnumName, FString enumStr) {
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
			SpawnParams.Owner = Cast<ACharacter>(inContextObject);
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
}