// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Froggo.h"
#include "FroggoCharacter.h"

AFroggoCharacter::AFroggoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	m_CanMove = true;
	m_playerAlive = true;
}

void AFroggoCharacter::addPullForce(int32 force)
{
	m_pullForce += force;
}

void AFroggoCharacter::pullTimerStart()
{
	// make sure pull force is how much needed to kill player
	m_pullForce = m_PullLossAmount;

	if (m_MaxPullTime <= 0.0)
	{
		m_MaxPullTime = 0.05;
	}

	// stop the player from moving
	m_CanMove = false;

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("PullTimer"));

	GetWorld()->GetTimerManager().SetTimer(m_PullTimerHandle,
		TimerDel, m_MaxPullTime, true);

}

void AFroggoCharacter::pullTimerEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(m_PullTimerHandle);
	m_CanMove = true;
	m_MaxPullTime -= m_TimeLoss;
}

bool AFroggoCharacter::GetPlayerAlive()
{
	return m_playerAlive;
}

FTimerHandle AFroggoCharacter::GetPullTimerHandle()
{
	return m_PullTimerHandle;
}

void AFroggoCharacter::PullTimer()
{
	if (m_pullForce > 0)
	{
		// player didn't break free
		UE_LOG(LogTemp, Warning, TEXT("YOU LOST!"));
		GetWorld()->GetTimerManager().ClearTimer(m_PullTimerHandle);
		m_playerAlive = false;
		return;
	}

	// player broke free
	// clear timer function
	m_CanMove = true;
	m_MaxPullTime -= m_TimeLoss;

	UE_LOG(LogTemp, Warning, TEXT("YOU BROKE FREE!"));
	GetWorld()->GetTimerManager().ClearTimer(m_PullTimerHandle);
	return;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFroggoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &AFroggoCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFroggoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFroggoCharacter::TouchStopped);
}

void AFroggoCharacter::MoveRight(float Value)
{
	if (m_CanMove)
	{
		// add movement in that direction
		AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
	}
}

void AFroggoCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	//Jump();
}

void AFroggoCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	//StopJumping();
}

