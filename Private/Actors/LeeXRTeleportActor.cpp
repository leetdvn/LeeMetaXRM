// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/LeeXRTeleportActor.h"

#include <HeadMountedDisplayFunctionLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ALeeXRTeleportActor::ALeeXRTeleportActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	NSTeleportRing = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSTeleportRing"));
	SetRootComponent(Root);

	NSPlayAreaBound = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NSPlayAreaBound"));
	SetRootComponent(Root);

}

// Called when the game starts or when spawned
void ALeeXRTeleportActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector2D AreaBounds = UHeadMountedDisplayFunctionLibrary::GetPlayAreaBounds(EHMDTrackingOrigin::Stage);
	NSPlayAreaBound->SetVariableVec3(TEXT("User.PlayAreaBounds"), FVector(AreaBounds.X, AreaBounds.Y, 0));
}

// Called every frame
void ALeeXRTeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APawn* pawn =  UGameplayStatics::GetPlayerPawn(World, 0);

	FVector CameeraLocation = UGameplayStatics::GetPlayerCameraManager(World, 0)->GetCameraLocation();

	FVector InverseVec = UKismetMathLibrary::InverseTransformLocation(pawn->GetTransform(), CameeraLocation);

	FVector NegateVec = UKismetMathLibrary::NegateVector(InverseVec);

	NSPlayAreaBound->SetRelativeLocation(FVector(NegateVec.X,NegateVec.Y,0));

	SetActorRotation(pawn->GetActorRotation());
}

