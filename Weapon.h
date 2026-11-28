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
	void WeaponTrace(FHitResult& OutHit, float TranceLength) const; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "FPS_WeaponType");
	FGameplayTag WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "FPS_Aiming");
	float AimFieldOfView;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = "FPS_Trace");
	float TraceRadius;
	
	void Local_Fire(const FVector& ImpactPoint, const FVector& ImpactNormal, TEnumAsByte<EPhysicalSurface> ImpactSurfaceType, bool bIsFirstPerson);




protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void FireEffects(const FVector& ImpactPoint, const FVector& ImpactNormal, EPhysicalSurface ImpactSurfaceType, bool bIsFirstPerson);
	
	// weapons mesh first person view
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "FPS_Weapon")
    	TObjectPtr<USkeletalMeshComponent> Mesh1P;
    	
    	// Weapons mesh third person view"
    	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "FPS_Weapon") 
    	TObjectPtr<USkeletalMeshComponent> Mesh3P;
	
	
private:
	
	void SetMeshVisibilies(APawn * OwningPawn) const; 
};
