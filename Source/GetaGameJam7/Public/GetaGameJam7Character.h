// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "GetaGameJam7Character.generated.h"

UENUM(BlueprintType)
enum class ECharacterAnimationState : uint8
{
	IDLE	UMETA(DisplayName = "Idle"),
	RUNNING	UMETA(DisplayName = "Running"),
	DEAD	UMETA(DisplayName = "Dead"),
	POTTED	UMETA(DisplayName = "Potted")
};

/**
 * This class is the default character for GetaGameJam7, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class AGetaGameJam7Character : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	virtual void Tick(float DeltaSeconds) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	ECharacterAnimationState AnimState;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play when dead
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	class UPaperFlipbook* DeadAnimation;

	// The animation to play when the game is won
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	class UPaperFlipbook* WinAnimation;

	// The water level the flower is currently at
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FlowFlow)
	float WaterLevel;

public:
	const float MaxWaterLevel = 1.0f;

protected:
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateAnimationState();

	void UpdateCharacter(float DeltaSeconds);

	void UpdateWaterLevel(float DeltaSeconds);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	AGetaGameJam7Character();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE const float GetWaterLevel() const { return WaterLevel; }

	const bool IsIdle() const;
	const bool IsDead() const;
	const bool IsPotted() const;
	const bool IsRunning() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// BlueprintNativeEvent to reset all player variables
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = FlowFlow)
	void Reset() override;

	// BlueprintNativeEvent to kill the player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = FlowFlow)
	void Kill();

	// BlueprintNativeEvent to execute when the player has won
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = FlowFlow)
	void Win();
};
