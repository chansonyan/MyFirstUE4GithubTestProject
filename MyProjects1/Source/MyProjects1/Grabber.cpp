// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	
	SetupInputComponent();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("UGrabber  "));
}


void UGrabber::Grab()
{
	///  LINE TRACE and see Try and reach any actor with physics body collision chanel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	// Get the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// if we hit something then we attach physics handle
	if (ActorHit)
	{
		mPhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,	// No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	// Release physics handle
	mPhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// if physics handle is attached
	if (mPhysicsHandle->GetGrabbedComponent())
	{
		// move the object that we are holding
		mPhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

/// Look for attached sibling Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	mPhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (mPhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component."), *(GetOwner()->GetName()));
	}
}

/// Look for attached sibling Input Component(Only appears at runtime)
void UGrabber::SetupInputComponent()
{
	mInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (mInputComponent)
	{
		/// Bind the Input Action
		mInputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		mInputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component."), *(GetOwner()->GetName()));
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{


	/*
	/// Draw a red trace in the world to visualize.

	DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(255, 0, 0),
	false,
	0.f,
	0.f,
	10.f
	);
	*/


	/// Line-trace (aka Ray-Cast) to Reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}


	return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
	/// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd() const
{
	/// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
