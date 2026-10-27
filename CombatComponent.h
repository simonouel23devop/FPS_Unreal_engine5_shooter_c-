// simon project fps

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	
	
	// Cycle to the weapons when switch in the inventory.
	
	void Initiate_CycleWeapon(); // switch weapons
	void Initiate_RunAction();
	void Initiate_FireWeapons_Pressed(); // press triggers mouse
	void Initiate_FireWeapons_Released(); // release triggers mouse
	void initiate_ReloadWeapon(); // reloading the weapons
	void initiate_Aim_Pressed(); // aiming press
	void initiate_Aim_Released(); // aiming release
	
	
	
	

protected:
	
	
private:
	
	
            
};
