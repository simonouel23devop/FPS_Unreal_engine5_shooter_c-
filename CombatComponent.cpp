//Simon project FPS


#include "CombatComponent.h"

#include "Engine/Engine.h"

UCombatComponent::UCombatComponent()
{
	

	PrimaryComponentTick.bCanEverTick = true;

	

}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::Initiate_CycleWeapon()
{
	
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("Initiate_CycleWeapon"), false);
}

void UCombatComponent::Initiate_RunAction()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("Initiate_RunAction"), false);
}

void UCombatComponent::Initiate_FireWeapons_Pressed()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("Initiate_FireWeapons_Pressed"), false);
}

void UCombatComponent::Initiate_FireWeapons_Released()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("Initiate_FireWeapons_Released"), false);
}

void UCombatComponent::initiate_ReloadWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("initiate_ReloadWeapon"), false);
}

void UCombatComponent::initiate_Aim_Pressed()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("initiate_Aim_Pressed"), false);
}

void UCombatComponent::initiate_Aim_Released()
{
	GEngine->AddOnScreenDebugMessage(-1 , 5.f , FColor::Cyan, TEXT("initiate_Aim_Released"), false);
}
