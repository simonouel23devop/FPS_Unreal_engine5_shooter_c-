//Simon project FPS


#include "CombatComponent.h"

#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInterface.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	
	TraceLength = 200000.0f;
	
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent, Inventory);
	DOREPLIFETIME(UCombatComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UCombatComponent, bAiming, COND_SkipOwner);
}

void UCombatComponent::Initiate_CycleWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_CycleWeapon"), false);
}

void UCombatComponent::Initiate_RunAction()
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		Server_SetMaxWalkSpeed(2000.0f);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_RunAction"), false);
}

void UCombatComponent::End_RunAction()
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		Server_SetMaxWalkSpeed(900.0f);
	}
}

void UCombatComponent::Server_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	if (NewSpeed != 900.0f && NewSpeed != 2000.0f)
	{
		return;
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

void UCombatComponent::Initiate_FireWeapons_Pressed()
{
	LocalFireWeapon();
}

void UCombatComponent::Initiate_FireWeapons_Released()
{
}

void UCombatComponent::Initiate_ReloadWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_ReloadWeapon"), false);
}

void UCombatComponent::Initiate_Aim_Pressed()
{
	Local_Aim(true);

	if (GetOwner() && GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		Server_SetAiming(true);
	}
}

void UCombatComponent::Initiate_Aim_Released()
{
	Local_Aim(false);

	if (GetOwner() && GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		Server_SetAiming(false);
	}
}

void UCombatComponent::ToggleAiming()
{
	Local_Aim(!bAiming);

	if (GetOwner() && GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		Server_SetAiming(bAiming);
	}
}

void UCombatComponent::Server_SetAiming_Implementation(bool bNewAiming)
{
	Local_Aim(bNewAiming);
	Multicast_Aim(bNewAiming);
}

void UCombatComponent::Server_Aim_Implementation(bool bPressed)
{
	Server_SetAiming(bPressed);
}

void UCombatComponent::Multicast_Aim_Implementation(bool bPressed)
{
	if (GetOwner() && GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		Local_Aim(bPressed);
	}
}

void UCombatComponent::OnRep_BAiming()
{
	Local_Aim(bAiming);
}

void UCombatComponent::Local_Aim(bool bPressed)
{
	bAiming = bPressed;
}

void UCombatComponent::LocalFireWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	ensure(IsValid(WeaponData));
	UAnimMontage* Montage1P = WeaponData->FirstPersonMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;

	// play the fire weapon montage for the first-person mesh
	USkeletalMeshComponent* Mesh1P = IPlayerInterface::Execute_GetMesh1P(GetOwner());
	if (IsValid(Montage1P) && IsValid(Mesh1P))
	{
		Mesh1P->GetAnimInstance()->Montage_Play(Montage1P);
	}
	
	FHitResult Hit;
	CurrentWeapon->WeaponTrace(Hit,TraceLength);
	
	Server_FireWeapon();
}

void UCombatComponent::Multicast_FireWeapon_Implementation()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (OwningPawn->IsLocallyControlled())
	{
		// do local stuff.
	}
	else
	{
		
		ensure(IsValid(WeaponData));
		UAnimMontage* Montage3P = WeaponData->ThirdPersonMontages.FindChecked(CurrentWeapon->WeaponType).FireMontage;

		// play the fire weapon montage for the first-person mesh
		USkeletalMeshComponent* Mesh3P = IPlayerInterface::Execute_GetMesh3P(GetOwner());
		if (IsValid(Montage3P) && IsValid(Mesh3P))
		{
			Mesh3P->GetAnimInstance()->Montage_Play(Montage3P);
		}
		
		
	}
}


AWeapon* UCombatComponent::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor)) return nullptr;
	if (OwningActor->GetLocalRole() < ROLE_Authority) return nullptr;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	return GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnInfo);
}

void UCombatComponent::Server_FireWeapon_Implementation()
{
	Multicast_FireWeapon();
}


void UCombatComponent::Equip(AWeapon* Weapon)
{
	if (!IsValid(Weapon))
	{
		CurrentWeapon = nullptr;
		return;
	}

	CurrentWeapon = Weapon;
	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToOwningPawn();
	}
}

void UCombatComponent::SpawnInventory() // spawn weapons
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor) || OwningActor->GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	for (const TSubclassOf<AWeapon>& WeaponClass : DefaultWeaponClasses)
	{
		if (AWeapon* Weapon = SpawnWeapon(WeaponClass))
		{
			Inventory.AddUnique(Weapon);
		}
	}

	if (Inventory.Num() > 0 && IsValid(Inventory[0]))
	{
		Equip(Inventory[0]); // equip weapons back to array 0.
	}
}

void UCombatComponent::DestroyInventory() // remove the weapons.
{
	for (AWeapon* Weapon : Inventory)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}

	Inventory.Empty();
}

void UCombatComponent::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	if (!IsValid(CurrentWeapon))
	{
		return;
	}

	CurrentWeapon->AttachToOwningPawn();
}
