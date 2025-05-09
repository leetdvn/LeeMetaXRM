// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeeXRActorUI.generated.h"


DEFINE_LOG_CATEGORY_STATIC(LogLeeXRActorUI, Log, All);


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LeeXR|Properties")
	int32 SnapCount = 3;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="inCarName"></param>
	/// <returns></returns>
	UStaticMeshComponent* FindCarComponentByName(const FString inCarName);

	template<typename T>
	T* FindStaticMeshComponentByName(const FString inCarName);

	UMaterialInstanceDynamic* FindMaterialByName(const FString inName);


	bool IsReadyToPlay() const
	{
		return bBodyReady && bLeftReady && bRightReady;
	}

	float PlayAnimation();
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

	TObjectPtr<class UBoxComponent> HandComponent;

	FTimerHandle HandTimerHandle;

	float HandVeryfied = 0.f;

	void InitDynamicMaterial();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bBodyReady = false;

	bool bLeftReady = false;

	bool bRightReady = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

template<typename T>
inline T* ALeeXRActorUI::FindStaticMeshComponentByName(const FString inCarName)
{
	TArray<UActorComponent*> Components = K2_GetComponentsByClass(T::StaticClass());
	for (auto Component : Components)
	{
		if(auto StaticMeshComponent = Cast<T>(Component))
		{
			if (StaticMeshComponent->GetName().StartsWith(inCarName) ||
				StaticMeshComponent->GetName().EndsWith(inCarName) ||
				StaticMeshComponent->GetName() == inCarName)
			{
				return StaticMeshComponent;
			}
			//UE_LOG(LogLeeXRActorUI, Log, TEXT("Component Name %s"), *StaticMeshComponent->GetName());
		}
	}
	return nullptr;
}

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
