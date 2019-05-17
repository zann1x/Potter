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

//////////////////////////////////////////////////////////////////////////
// Animation

void AGetaGameJam7Character::UpdateAnimation()
{
	const float PlayerSpeedSqr = GetVelocity().SizeSquared();

	// Are we moving or standing still?
	if (AnimState != AnimationState::DEAD && AnimState != AnimationState::POTTED)
	{
		if (PlayerSpeedSqr > 0.0f)
			AnimState = AnimationState::RUNNING;
		else
			AnimState = AnimationState::IDLE;
	}

	UPaperFlipbook* DesiredAnimation = nullptr;
	switch (AnimState)
	{
	case AnimationState::IDLE:
		DesiredAnimation = IdleAnimation;
		break;
	case AnimationState::RUNNING:
		DesiredAnimation = RunningAnimation;
		break;
	case AnimationState::DEAD:
		DesiredAnimation = DeadAnimation;
		break;
	case AnimationState::POTTED:
		DesiredAnimation = WinAnimation;
		break;
	}

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AGetaGameJam7Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGetaGameJam7Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGetaGameJam7Character::TouchStopped);
}

void AGetaGameJam7Character::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AGetaGameJam7Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AGetaGameJam7Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AGetaGameJam7Character::Kill_Implementation()
{
	AnimState = AnimationState::DEAD;
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	UpdateAnimation();
}

void AGetaGameJam7Character::Win_Implementation()
{
	AnimState = AnimationState::POTTED;
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	UpdateAnimation();
}

void AGetaGameJam7Character::Reset_Implementation()
{
	WaterLevel = MaxWaterLevel;
	AnimState = AnimationState::IDLE;
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	SetActorLocation(Actors[0]->GetActorLocation(), false, nullptr, ETeleportType::ResetPhysics);
	SetActorTickEnabled(true);
}

void AGetaGameJam7Character::UpdateCharacter(float DeltaSeconds)
{
	if (AnimState != AnimationState::DEAD)
	{
		WaterLevel -= (DeltaSeconds / 15.0f);
		if (WaterLevel <= 0.0f)
		{
			Kill();
		}
	}
	
	// Update animation to match the motion
	UpdateAnimation();

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
