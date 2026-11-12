// Simon fps project


#include "Weapon.h"

#include "PlayerInterface.h"
#include "Camera/CameraComponent.h"

AWeapon::AWeapon()
{
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true; // replication in game for the mesh weapons
	bNetUseOwnerRelevancy = true;


	// mesh 1p visibility in game for weapons
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh1P"));
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetHiddenInGame(true);
    SetRootComponent(Mesh1P);
	
	
	
	// mesh visibility in game for weapons
	
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh3P"));
	Mesh3P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;	
	Mesh3P->bReceivesDecals = false;
	Mesh3P->CastShadow = true;
	Mesh3P->SetupAttachment(Mesh1P);
	Mesh3P->SetHiddenInGame(true);
	
	AimFieldOfView = 65.0F;
}

void AWeapon::OnRep_Instigator()
{
	Super::OnRep_Instigator();
	AttachToOwningPawn();
	
	
}

USkeletalMeshComponent* AWeapon::GetMesh1P() const  // for mesh1p
{
	return Mesh1P;
	
	
}

USkeletalMeshComponent* AWeapon::GetMesh3P() const // for mesh3p
{
	
	return Mesh3P;
	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::AttachToOwningPawn() const // attach grip point weapons to player
{
	
	APawn* OwningPawn = GetInstigator();
	if (!IsValid(OwningPawn) || !OwningPawn->Implements<UPlayerInterface>()) return;
	
	SetMeshVisibilies(OwningPawn);
	
	
	const FName AttachPoint = IPlayerInterface::Execute_GetWeaponAttachPoint(OwningPawn, WeaponType);
	USkeletalMeshComponent* PawnMesh1P = IPlayerInterface::Execute_GetMesh1P(OwningPawn);
	USkeletalMeshComponent* PawnMesh3P = IPlayerInterface::Execute_GetMesh3P(OwningPawn);
	
	Mesh1P->AttachToComponent(PawnMesh1P, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
	Mesh3P->AttachToComponent(PawnMesh3P, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
	
	
	
}

void AWeapon::SetMeshVisibilies(APawn* OwningPawn) const
{
	if (OwningPawn->IsLocallyControlled())
		
	{
		Mesh1P->SetHiddenInGame(false);
		Mesh3P->SetHiddenInGame(true);
	}
	else
	{
		Mesh1P->SetHiddenInGame(true);
		Mesh3P->SetHiddenInGame(false);
		
	}
	
	
}


