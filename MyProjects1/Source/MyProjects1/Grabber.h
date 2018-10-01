// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECTS1_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far ahead of the player can we reach in cm
	float Reach = 100.f;
	
	UPhysicsHandleComponent* mPhysicsHandle = nullptr;

	UInputComponent* mInputComponent = nullptr;

	// Ray-Cast and Grab what's in reach.
	void Grab();

	// Called when the Grab key is Released
	void Release();

	// Find (assumed) attached Physics handle
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached Input Component
	void SetupInputComponent();

	// Return Hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Returns Current Start of Reach Line
	FVector GetReachLineStart() const;

	// Returns Current End of Reach Line
	FVector GetReachLineEnd() const;
};
