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

	FVector position;
	FRotator rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT position, OUT rotation);

	UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"),
		*position.ToString(), *rotation.ToString());

	FVector lineEnd = position + (rotation.Vector() * Reach);

	DrawDebugLine(GetWorld(), position, lineEnd, FColor(255, 0, 0), false, 0.f, 0, 10.f);

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
}

