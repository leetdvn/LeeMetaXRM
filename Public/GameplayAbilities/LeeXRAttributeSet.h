// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LeeXRAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class LEEMETAXRM_API ULeeXRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = "LeeVR Settings|Tags")
	FGameplayAttributeData GrabableTag;
	ATTRIBUTE_ACCESSORS(ULeeXRAttributeSet, GrabableTag);

};
