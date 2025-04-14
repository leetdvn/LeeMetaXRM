// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LeeXRInterfaceHMD.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULeeXRInterfaceHMD : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEEMETAXRM_API ILeeXRInterfaceHMD
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnHMDOrientReset() = 0;

	/// <summary>
	/// Native Event Blueprint Implement
	/// </summary>
	/// <param name="NewLevelName"></param>
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "LeeXR|Func")
	void OnHMDLevelChanged(const FString& NewLevelName);
};
