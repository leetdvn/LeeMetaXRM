#pragma once

#include <Async/AsyncWork.h>
#include "CoreMinimal.h"
#include "UObject/Interface.h"

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
}