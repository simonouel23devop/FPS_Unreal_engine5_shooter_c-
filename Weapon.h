// simon project

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void OnRep_Instigator() override;
	USkeletalMeshComponent* GetMesh1P() const;
	USkeletalMeshComponent* GetMesh3P() const;
	void AttachToOwningPawn() const;
	
protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, Category = "FPS_WeaponType");
	FGameplayTag WeaponType;
	
	
	
private:
	
	// weapons mesh first person view
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	// Weapons mesh third person view"
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh3P;
	
	void SetMeshVisibilies(APawn * OwningPawn) const; 
};
