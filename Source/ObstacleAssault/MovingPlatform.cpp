// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{

	Super::BeginPlay();

	GetWorld()->Exec(GetWorld(), TEXT("r.BloomQuality 0"));

	GEngine->SetMaxFPS(60.0f);

	if (FPSWidgetClass)
	{
		FPSWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), FPSWidgetClass);
		if (FPSWidgetInstance)
		{
			FPSWidgetInstance->AddToViewport();
		}
	}

	// Store initial state for movement and rotation logic
	globalStartLocation = GetActorLocation();
	initialMoveDirection = platformVelocity.GetSafeNormal();
	InitialRotation = GetActorRotation();
	initialLocation = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandlePlatformTrapActivation(DeltaTime);

	// Handle trap logic if this actor is configured as a trap
	if (!bIsTrap)
	{
		if (bShouldMove)
		{
			MovePlatform(DeltaTime);
		}

		if (bShouldRotate)
		{
			RotatePlatform(DeltaTime);
		}
	}
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{

	FVector CurrentLocation = GetActorLocation();

	// Calculate displacement along the original movement direction
	float CurrentOffset = FVector::DotProduct(CurrentLocation - initialLocation, initialMoveDirection);
	float VelocityDir = FVector::DotProduct(platformVelocity, initialMoveDirection);

	// Reverse direction at boundaries
	if (CurrentOffset >= moveToADistance && VelocityDir > 0)
	{
		platformVelocity = -platformVelocity;
	}
	else if (CurrentOffset <= -moveToBDistance && VelocityDir < 0)
	{
		platformVelocity = -platformVelocity;
	}

	// Apply movement
	FVector NewLocation = CurrentLocation + (platformVelocity * DeltaTime);
	SetActorLocation(NewLocation);
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(platformRotationSpeed * DeltaTime);
}

void AMovingPlatform::HandlePlatformTrapActivation(float DeltaTime)
{
	APawn *PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
		return;

	/**
	 * FIX: Using globalStartLocation instead of current ActorLocation.
	 * This prevents a feedback loop where the platform's own rotation
	 * changes the calculated distance to the player, causing "twitching".
	 */
	float Distance = FVector::Dist(globalStartLocation, PlayerPawn->GetActorLocation());

	/**
	 * LATCH LOGIC WITH HYSTERESIS:
	 * We trigger the trap at ActivationDistance.
	 * We only reset it when the player is much further away.
	 * This prevents rapid on/off toggling when the player is at the edge.
	 */

	if (!bTrapTriggered && Distance <= ActivationDistance)
	{
		bTrapTriggered = true;
		UE_LOG(LogTemp, Warning, TEXT("[%s] ARMADILHA ATIVADA!"), *GetName());
	}
	else if (bTrapTriggered && Distance > ActivationDistance + 200.0f)
	{
		bTrapTriggered = false;
		UE_LOG(LogTemp, Display, TEXT("[%s] ARMADILHA RESETADA."), *GetName());
	}

	// Execution of the rotation based on proximity state
	if (ActivationType == EPlatformActivationType::Proximity)
	{

		// Determine the goal rotation
		FRotator TargetRotation = bTrapTriggered ? (InitialRotation + TrapRotation) : InitialRotation;

		/**
		 * STABILITY FIX: Using Quaternions (FQuat) instead of Rotators (FRotator).
		 * Quaternions avoid "Gimbal Lock" and ensure the smoothest, most direct path
		 * between two rotations, preventing the "spinning top" (pião) effect.
		 */
		FQuat CurrentQuat = GetActorQuat();
		FQuat TargetQuat = TargetRotation.Quaternion();

		// QInterpTo is the gold standard for stable rotation interpolation
		FQuat NewQuat = FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, RotationSpeed);

		SetActorRotation(NewQuat);
	}
}

float AMovingPlatform::GetDistanceMoved()
{
	return FVector::Distance(startLocation, GetActorLocation());
}
