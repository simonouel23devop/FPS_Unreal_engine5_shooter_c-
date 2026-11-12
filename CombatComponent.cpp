//Simon project FPS


#include "CombatComponent.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_RunAction"), false);
}

void UCombatComponent::Initiate_FireWeapons_Pressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_FireWeapons_Pressed"), false);
}

void UCombatComponent::Initiate_FireWeapons_Released()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Initiate_FireWeapons_Released"), false);
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
