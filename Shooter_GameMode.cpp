// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter_GameMode.h"

#include "Shooter_character.h"
#include "ShooterPlayerController.h"

AShooter_GameMode::AShooter_GameMode()
{
	DefaultPawnClass = AShooter_character::StaticClass();
	PlayerControllerClass = AShooterPlayerController::StaticClass();
}
