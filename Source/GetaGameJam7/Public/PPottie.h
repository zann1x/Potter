// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPottie.generated.h"

UCLASS()
class GETAGAMEJAM7_API APPottie : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pottie, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* PaperSprite;
	
	UFUNCTION()
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// BlueprintNativeEvent to reset all player variables
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Pottie)
	void Reset() override;

public:	
	// Sets default values for this actor's properties
	APPottie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
