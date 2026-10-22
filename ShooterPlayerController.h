// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class FPS_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputMappingContext> ShooterIMC;

	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> JumpAction;

	void Input_Crouch();
	void Input_Jump();
	void Input_StopJumping();
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
};
