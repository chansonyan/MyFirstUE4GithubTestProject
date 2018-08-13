// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorComp.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoorComp::UOpenDoorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//GetWorld();

	ActorToOpen = GetWorld()->GetFirstPlayerController()->GetPawn();

	OwnerDoor = GetOwner();

	UE_LOG(LogTemp, Warning, TEXT("UOpenDoorComp "));
}


// Called every frame
void UOpenDoorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// Poll the Trigger Volume
	// If the ActorsToOpen is in the Volume
	if ( PressurePlate->IsOverlappingActor(ActorToOpen) )
	{
		OpenDoor();

		// Update time
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("LastDoorOpenTime : %f "), LastDoorOpenTime);
	}

	// Check if it's time to close the door
	if ( (GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelayTime)
	{
		CloseDoor();
	}

}

void UOpenDoorComp::OpenDoor()
{
	// Set the door rotation
	OwnerDoor->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoorComp::CloseDoor()
{
	// Set the door rotation
	OwnerDoor->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}