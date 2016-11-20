// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	APlayerController *player = GetWorld()->GetFirstPlayerController();

	FVector vector;
	FRotator rotation;
	player->GetPlayerViewPoint(OUT vector, OUT rotation);

	UE_LOG(LogTemp, Warning, TEXT("Player x=%f,y=%f,z=%f p=%f,y=%f,r=%f\n"),
		vector.X, vector.Y, vector.Z,
		rotation.Pitch, rotation.Yaw, rotation.Roll);
}

