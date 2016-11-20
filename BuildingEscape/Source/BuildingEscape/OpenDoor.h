// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	void OpenDoor();
	void CloseDoor();

	UPROPERTY(VisibleAnywhere)
		float OpenAngle = -160.0f;

	UPROPERTY(VisibleAnywhere)
		float CloseAngle = -90.0f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
		float DoorOpenDelay = 5;

	float DoorLastOpenTime;

	AActor* ActorThatOpens;

	AActor* Owner;

};
