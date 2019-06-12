// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GetaGameJam7Character.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGetaGameJam7Character

AGetaGameJam7Character::AGetaGameJam7Character()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(30.0f);
	GetCapsuleComponent()->SetCapsuleRadius(3.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 640.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
	GetCharacterMovement()->MaxStepHeight = 2.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set other defaults
	WaterLevel = MaxWaterLevel;
}

void AGetaGameJam7Character::Kill_Implementation()
{
	AnimState = EAnimationState::DEAD;
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	UpdateAnimation();
}

void AGetaGameJam7Character::Win_Implementation()
{
	AnimState = EAnimationState::POTTED;
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	UpdateAnimation();
}

void AGetaGameJam7Character::Reset_Implementation()
{
	// Don't call the super method here to avoid weird side effects of the default's engine implementation

	WaterLevel = MaxWaterLevel;
	AnimState = EAnimationState::IDLE;
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	SetActorLocation(Actors[0]->GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
	SetActorTickEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
// Animation / Updates

void AGetaGameJam7Character::UpdateAnimation()
{
	const float PlayerSpeedSqr = GetVelocity().SizeSquared();

	// Are we moving or standing still?
	if (AnimState != EAnimationState::DEAD && AnimState != EAnimationState::POTTED)
	{
		if (PlayerSpeedSqr > 0.0f)
			AnimState = EAnimationState::RUNNING;
		else
			AnimState = EAnimationState::IDLE;
	}

	UPaperFlipbook* DesiredAnimation = nullptr;
	switch (AnimState)
	{
	case EAnimationState::IDLE:
		DesiredAnimation = IdleAnimation;
		break;
	case EAnimationState::RUNNING:
		DesiredAnimation = RunningAnimation;
		break;
	case EAnimationState::DEAD:
		DesiredAnimation = DeadAnimation;
		break;
	case EAnimationState::POTTED:
		DesiredAnimation = WinAnimation;
		break;
	}

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AGetaGameJam7Character::UpdateCharacter(float DeltaSeconds)
{
	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AGetaGameJam7Character::UpdateWaterLevel(float DeltaSeconds)
{
	if (AnimState != EAnimationState::DEAD)
	{
		WaterLevel -= (DeltaSeconds / 15.0f);
		if (WaterLevel <= 0.0f)
		{
			Kill();
		}
	}
}

void AGetaGameJam7Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update health level of the character
	UpdateWaterLevel(DeltaSeconds);

	// Update animation to match the motion
	UpdateAnimation();

	// Update the facing direction of the character
	UpdateCharacter(DeltaSeconds);
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGetaGameJam7Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGetaGameJam7Character::MoveRight);
}

void AGetaGameJam7Character::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

/////////////////////////////
// AUTOMATION TEST METHODS //
/////////////////////////////

bool AGetaGameJam7Character::AutomationIsDead()
{
	return AnimState == EAnimationState::DEAD;
}

bool AGetaGameJam7Character::AutomationIsIdle()
{
	return AnimState == EAnimationState::IDLE;
}

bool AGetaGameJam7Character::AutomationIsPotted()
{
	return AnimState == EAnimationState::POTTED;
}

bool AGetaGameJam7Character::AutomationHasFullHealth()
{
	return WaterLevel == MaxWaterLevel;
}
