// simon project fps

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponData.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class AWeapon;
class UWeaponData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	// Cycle to the weapons when switch in the inventory.
	
	void Initiate_CycleWeapon(); // switch weapons
	void Initiate_RunAction();
	void Initiate_FireWeapons_Pressed(); // press triggers mouse
	void Initiate_FireWeapons_Released(); // release triggers mouse
	void Initiate_ReloadWeapon(); // reloading the weapons
	void Initiate_Aim_Pressed(); // aiming press
	void Initiate_Aim_Released(); // aiming release
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "FPS_Weapon")
	TObjectPtr<UWeaponData> WeaponData;
	
	void Equip(AWeapon* Weapon);
	void SpawnInventory();
	void DestroyInventory();

	UFUNCTION(BlueprintCallable)
	void ToggleAiming();

	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bNewAiming);

	UFUNCTION(Server, Reliable)
	void Server_Aim(bool bPressed);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Aim(bool bPressed);
	
	UFUNCTION()
	void OnRep_BAiming();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BAiming)
	bool bAiming = false;
	
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentWeapon)
	TObjectPtr<AWeapon> CurrentWeapon;
protected:
	
		

private:



	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);
	
	UPROPERTY(Transient, Replicated)
	TArray<AWeapon*> Inventory;
	
	UPROPERTY(EditDefaultsOnly , Category= "FPS_Weapon")
	TArray<TSubclassOf<AWeapon>> DefaultWeaponClasses; // not array yet for player inventory
	
	
	// spawning player weapons
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponClass) const;
	

	void Local_Aim(bool bPressed);
	
	
	
	
};
