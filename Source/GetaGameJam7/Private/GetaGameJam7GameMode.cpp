// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GetaGameJam7GameMode.h"
#include "GetaGameJam7Character.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AGetaGameJam7GameMode::AGetaGameJam7GameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGetaGameJam7Character::StaticClass();	
}

void AGetaGameJam7GameMode::Reset_Implementation()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("GameActor")), Actors);
	for (AActor* Actor : Actors)
	{
		Actor->Reset();
	}
}
