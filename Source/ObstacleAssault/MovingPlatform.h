// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UENUM(BlueprintType)
enum class EPlatformActivationType : uint8
{
	Triggered UMETA(DisplayName = "Trigger Once"),
	Proximity UMETA(DisplayName = "Proximity")
};

UCLASS()
class OBSTACLEASSAULT_API AMovingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);

	void HandlePlatformTrapActivation(float DeltaTime);

	float GetDistanceMoved();

	FVector globalStartLocation;

	FVector initialLocation;

	FVector initialMoveDirection;

	FRotator InitialRotation;

	FVector startLocation;
	float distanceMoved = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation Platform")
	bool bShouldRotate = false;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	bool bShouldMove = false;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float moveToADistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float moveToBDistance = 0.0f;

	UPROPERTY(EditAnywhere)
	FVector platformVelocity;

	UPROPERTY(EditAnywhere)
	FRotator platformRotationSpeed;

	UPROPERTY(EditAnywhere)
	float moveDistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Trap",
						meta = (EditCondition = "bIsTrap", EditConditionHides))
	EPlatformActivationType ActivationType = EPlatformActivationType::Triggered;

	UPROPERTY(EditAnywhere, Category = "Trap",
						meta = (EditCondition = "bIsTrap", EditConditionHides))
	float ActivationDistance = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Trap")
	bool bIsTrap = false;

	UPROPERTY(EditAnywhere, Category = "Trap")
	FRotator TrapRotation = FRotator(-100.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Trap")
	float RotationSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> FPSWidgetClass;

	UPROPERTY()
	class UUserWidget *FPSWidgetInstance;

private:
	bool bTrapTriggered = false;

	bool bWasPlayerInRange = false;
};
