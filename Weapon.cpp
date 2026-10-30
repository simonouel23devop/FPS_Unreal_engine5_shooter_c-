// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Camera/CameraComponent.h"

AWeapon::AWeapon()
{
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // replication in game for the mesh weapons
	bNetUseOwnerRelevancy = true;


	// mesh 1p visibility in game
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh1P"));
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetHiddenInGame(true);
    SetRootComponent(Mesh1P);
	
	
	
	// mesh visibility in game
	
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh3P"));
	Mesh3P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;	
	Mesh3P->bReceivesDecals = false;
	Mesh3P->CastShadow = true;
	Mesh3P->SetupAttachment(Mesh1P);
	Mesh3P->SetHiddenInGame(true);
	
	
}

USkeletalMeshComponent* AWeapon::GetMesh1P() const
{
	return Mesh1P;
	
	
}

USkeletalMeshComponent* AWeapon::GetMesh3P() const
{
	
	return Mesh3P;
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


