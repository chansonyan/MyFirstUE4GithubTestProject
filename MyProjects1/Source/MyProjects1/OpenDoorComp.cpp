// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorComp.h"
#include "GameFramework/Actor.h"

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
	
	// Finding the Owning Actor
	AActor* OwnerActor = GetOwner();

	// Create a Rotator
	FRotator NewRotation = FRotator(0.f, 60.f, 0.f);

	// Set the door rotation
	OwnerActor->SetActorRotation(NewRotation);


	UE_LOG(LogTemp, Warning, TEXT("UOpenDoorComp "));
}


// Called every frame
void UOpenDoorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

