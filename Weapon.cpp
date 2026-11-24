// Simon fps project


#include "Weapon.h"

#include "../FPS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetTraceUtils.h"
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
	TraceRadius = 5.f;
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

void AWeapon::WeaponTrace(FHitResult& OutHit, float TranceLength) const
{
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_PhysicsBody, ECR_Block);

	ensure(GetInstigator());
	if (APlayerController* PC = Cast<APlayerController>(GetInstigator()->GetController()); IsValid(PC))
	{
		FVector EyesWorldLocation;
		FRotator EyesWorldRotation;
		PC->GetActorEyesViewPoint(EyesWorldLocation, EyesWorldRotation);
		const FVector EyesWorldDirection = UKismetMathLibrary::GetForwardVector(EyesWorldRotation);
		const FVector Start = EyesWorldLocation;
		const FVector End = Start + EyesWorldDirection * TranceLength;
		const bool bHit = GetWorld()->SweepSingleByChannel(
			OutHit,
			Start,
			End,
			FQuat::Identity,
			FPSTraceChannels::ECC_Weapon,
			FCollisionShape::MakeSphere(TraceRadius),
			QueryParams,
			ResponseParams);
		DrawDebugSphereTraceSingle(
			GetWorld(),
			Start,
			End,
			TraceRadius,
			EDrawDebugTrace::ForDuration,
			bHit,
			OutHit,
			FColor::Green,
			FColor::Red,
			5.0f);
	}
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
