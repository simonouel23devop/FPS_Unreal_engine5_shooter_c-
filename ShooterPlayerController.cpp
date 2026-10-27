// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterPlayerController.h"

#include "CombatComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"

AShooterPlayerController::AShooterPlayerController()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionAsset(
		TEXT("/Game/FPS/Input/InputAction/IA_Run.IA_Run"));
	if (RunActionAsset.Succeeded())
	{
		RunAction = RunActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ShooterMappingContextAsset(
		TEXT("/Game/FPS/Input/IMC_Shooter.IMC_Shooter"));
	if (ShooterMappingContextAsset.Succeeded())
	{
		ShooterIMC = ShooterMappingContextAsset.Object;
	}
}

void AShooterPlayerController::BeginPlay() // when begin play
{
	Super::BeginPlay();

	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		if (IsValid(ShooterIMC) && IsValid(RunAction))
		{
			const bool bRunActionMappedToLeftShift = ShooterIMC->GetMappings().ContainsByPredicate(
				[this](const FEnhancedActionKeyMapping& Mapping)
				{
					return Mapping.Action == RunAction && Mapping.Key == EKeys::LeftShift;
				});
			if (!bRunActionMappedToLeftShift)
			{
				ShooterIMC->MapKey(RunAction, EKeys::LeftShift);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Run input is unavailable: ShooterIMC or RunAction did not load."));
		}

		if (IsValid(ShooterIMC))
		{
			Subsystem->AddMappingContext(ShooterIMC, 0);
		}
	}
}

void AShooterPlayerController::SetupInputComponent() // input action component
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::Input_StopJumping);
	ShooterInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::Input_Crouch);
	ShooterInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ThisClass::Input_Run);
	ShooterInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ThisClass::Input_StopRunning);
}

void AShooterPlayerController::Input_Run()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!IsValid(ControlledCharacter))
	{
		return;
	}

	UCharacterMovementComponent* MovementComponent = ControlledCharacter->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed = RunSpeed;
	if (UCombatComponent* CombatComponent = ControlledCharacter->FindComponentByClass<UCombatComponent>())
	{
		CombatComponent->Initiate_RunAction();
	}
}

void AShooterPlayerController::Input_StopRunning()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
	}
}

void AShooterPlayerController::Input_Crouch()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!IsValid(ControlledCharacter))
	{
		return;
	}

	if (ControlledCharacter->bIsCrouched)
	{
		ControlledCharacter->UnCrouch();
	}
	else
	{
		ControlledCharacter->Crouch();
	}
}

void AShooterPlayerController::Input_Jump()
{
	ACharacter* ControlledCharacter = GetCharacter();
	if (!IsValid(ControlledCharacter))
	{
		return;
	}

	if (ControlledCharacter->bIsCrouched)
	{
		ControlledCharacter->UnCrouch();
	}
	else
	{
		ControlledCharacter->Jump();
	}
}

void AShooterPlayerController::Input_StopJumping()
{
	if (ACharacter* ControlledCharacter = GetCharacter())
	{
		ControlledCharacter->StopJumping();
	}
}

void AShooterPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AShooterPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(InputAxisVector.X);
	AddPitchInput(InputAxisVector.Y);
}
