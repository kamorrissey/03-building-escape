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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

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



void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics handle found for actor %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle NOT found for actor %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found for actor %s"), *GetOwner()->GetName());

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component NOT found for actor %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector position;
	FVector lineEnd;
	GetLineTrace(OUT position, OUT lineEnd);

	FCollisionQueryParams collisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		position,
		lineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		collisionQueryParams
	);

	AActor* hitActor = hit.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit actor %s"), *hitActor->GetName());
	}

	return hit;
}

void UGrabber::GetLineTrace(FVector& lineStart, FVector& lineEnd)
{
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT lineStart, OUT rotation);

	lineEnd = lineStart + (rotation.Vector() * Reach);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();

	/// If we hit something then attach a physics handle
	if (ComponentToGrab)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	PhysicsHandle->ReleaseComponent();
}

