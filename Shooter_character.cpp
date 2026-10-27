// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter_character.h"

#include "CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AShooter_character::AShooter_character()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.f;
	SpringArm->bUsePawnControlRotation = true;

	// camera first person
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(SpringArm);
	FirstPersonCamera->bUsePawnControlRotation = false;

	// arms mesh1p behind
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh1P"));
	Mesh1P->SetupAttachment(FirstPersonCamera);
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = true; // decals like blood
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	// visibility to owner and client :
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bReceivesDecals = true;


	Combat = CreateDefaultSubobject<UCombatComponent>("Combat");
	Combat->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AShooter_character::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
}



// Called every frame
void AShooter_character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooter_character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent *ShooterInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	ShooterInputComponent->BindAction(CycleWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_CycleWeapon);
	ShooterInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_FireWeapon_Pressed);
	ShooterInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Completed, this, &AShooter_character::Input_FireWeapon_Released);
	ShooterInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_Aim_Pressed);
	ShooterInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_Aim_Released);
	ShooterInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_ReloadWeapon);

}

void AShooter_character::Input_CycleWeapon()
{
	if (!Combat)
	{
		return;
	}

	Combat->Initiate_CycleWeapon();
}

void AShooter_character::Input_ReloadWeapon()
{
	if (!Combat)
	{
		return;
	}

	Combat->initiate_ReloadWeapon();
}

void AShooter_character::Input_FireWeapon_Pressed()
{
	if (!Combat)
	{
		return;
	}

	Combat->Initiate_FireWeapons_Pressed();
}

void AShooter_character::Input_FireWeapon_Released()
{
	if (!Combat)
	{
		return;
	}

	Combat->Initiate_FireWeapons_Released();
}

void AShooter_character::Input_Aim_Pressed()
{
	if (!Combat)
	{
		return;
	}

	Combat->initiate_Aim_Pressed();
}

void AShooter_character::Input_Aim_Released()
{
	if (!Combat)
	{
		return;
	}

	Combat->initiate_Aim_Released();
}