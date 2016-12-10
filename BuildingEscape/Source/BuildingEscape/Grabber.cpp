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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// if the physics handle is attached then move the object we're holding
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector lineStart;
		FVector lineEnd;
		GetLineTrace(OUT lineStart, OUT lineEnd);
		PhysicsHandle->SetTargetLocation(lineEnd);
	}
}

/// Find the PhysicsHandle component attached to the owning actor
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle NOT found for actor %s"), *GetOwner()->GetName());
	}
}

/// Get the InputComponent attached to the owning actor and bind triggers to it
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component NOT found for actor %s"), *GetOwner()->GetName());
	}
}

/// Get the first PhysicsBody within reach based on ray tracing
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector lineStart;
	FVector lineEnd;
	GetLineTrace(OUT lineStart, OUT lineEnd);

	FCollisionQueryParams collisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		lineStart,
		lineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		collisionQueryParams
	);

	return hit;
}

/// Get the start and end points of a line-trace within a reach distance
void UGrabber::GetLineTrace(FVector& lineStart, FVector& lineEnd)
{
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT lineStart, OUT rotation);

	lineEnd = lineStart + (rotation.Vector() * Reach);
}

/// Attach a PhysicsHandle to an actor in reach
void UGrabber::Grab()
{
	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();

	/// If we hit something then attach a physics handle
	if (ComponentToGrab)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

/// Detach a PhysicsHandle
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

