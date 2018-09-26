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
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// if we hit something then we attach physics handle
	if (ActorHit)
	{
		// attach physics handle
		mPhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}


	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed. "));
}

void UGrabber::Release()
{
	// Release physics handle
	mPhysicsHandle->ReleaseComponent();

	UE_LOG(LogTemp, Warning, TEXT("Grab Released. "));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if physics handle is attached
	if (mPhysicsHandle->GetGrabbedComponent())
	{
		// move the object that we are holding
		mPhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		

	
}

/// Look for attached sibling Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{

	mPhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (mPhysicsHandle)
	{
		;
	}
	else
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
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found."));

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

	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	// TODO Log out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Location : %s, Position: %s"),
	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString()
	);*/

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

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

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (aka Ray-Cast) to Reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		PlayerViewPointLocation,
		LineTraceEnd,
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
