// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Logging/StructuredLog.h"
#include "Kismet/GameplayStatics.h"


ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TankMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATank::Fire);
	}
}

void ATank::Move(const FInputActionValue& Value)
{
	FVector input = Value.Get<FVector>();
	float deltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	
	Movement(input, deltaTime);	
	Turn(input, deltaTime);
}

void ATank::Movement(FVector& input, float deltaTime)
{
	FVector deltaMove = FVector::ZeroVector;
	deltaMove.X = input.Y * MoveSpeed * deltaTime;
	AddActorLocalOffset(deltaMove, true);
}

void ATank::Turn(FVector& input, float deltaTime)
{
	FRotator deltaRotation = FRotator::ZeroRotator;
	deltaRotation.Yaw = input.X * TurnSpeed * deltaTime;
	AddActorLocalRotation(deltaRotation);
}

void ATank::Fire(const FInputActionValue& Value)
{
	bool input = Value.Get<bool>();
	if (input)
	{
		UE_LOGFMT(LogTemp, Display, "Fire :'{0}'", input);
	}
}
