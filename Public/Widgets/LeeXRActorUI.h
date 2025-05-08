// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRActorUI.generated.h"

UCLASS()
class LEEMETAXRM_API ALeeXRActorUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeeXRActorUI();

	UFUNCTION(BlueprintCallable, Category = "LeeXR|UI")
	UStaticMeshComponent* FindComponentByName(FString inName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR|Properties")
	float SinValue=10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR|Properties")
	float SinSpeed = 150.f;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="inCarName"></param>
	/// <returns></returns>
	UStaticMeshComponent* FindCarComponentByName(const FString inCarName);

	UMaterialInstanceDynamic* FindMaterialByName(const FString inName);

	void PlayAnimation();
	void StopAnimation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	template<typename T>
	TArray<T*> GetComponentsByClass();


	template<typename T>
	TArray<T*> GetComponentsByClass(const FString inStartStr);


	TArray<UStaticMeshComponent*> CarComponents;

	TObjectPtr<class UBoxComponent> Box;

	void InitDynamicMaterial();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

template<typename T>
inline TArray<T*> ALeeXRActorUI::GetComponentsByClass()
{
	TArray<UActorComponent*> Components = K2_GetComponentsByClass(T::StaticClass());
	TArray<T*> Results = TArray<T*>();
	for (auto Component : Components)
	{
		T* StaticMeshComponent = Cast<T>(Component);
		if (StaticMeshComponent)
		{
			Results.Add(StaticMeshComponent);
		}
	}
	return Results;
}

template<typename T>
inline TArray<T*> ALeeXRActorUI::GetComponentsByClass(const FString inStartStr)
{
	TArray<UActorComponent*> Components = K2_GetComponentsByClass(T::StaticClass());
	TArray<T*> Results = TArray<T*>();

	for (auto Component : Components)
	{
		T* StaticMeshComponent = Cast<T>(Component);
		if (StaticMeshComponent && StaticMeshComponent->GetName().StartsWith(inStartStr))
		{
			Results.Add(StaticMeshComponent);
		}
	}

	return Results;
}
