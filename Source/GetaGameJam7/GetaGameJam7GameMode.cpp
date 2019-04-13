// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GetaGameJam7GameMode.h"
#include "GetaGameJam7Character.h"

AGetaGameJam7GameMode::AGetaGameJam7GameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGetaGameJam7Character::StaticClass();	
}
