// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameFramework/Character.h"
#include "Shooter_character.generated.h"

class UInputAction;
class UCombatComponent;
class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;



UCLASS()
class FPS_API AShooter_character : public ACharacter , public IPlayerInterface
{
	GENERATED_BODY()

public:
	AShooter_character();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	// player interface
    	virtual FName GetWeaponAttachPoint_Implementation(const FGameplayTag& WeaponType) const override;
    	virtual USkeletalMeshComponent* GetMesh1P_Implementation() const override;
    	virtual USkeletalMeshComponent* GetMesh3P_Implementation() const override;
		virtual void BeginPlay() override;
    	virtual void BeginDestroy() override;
	
protected:
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "FPS_Combat");
    	TObjectPtr<UCombatComponent> Combat;
	
	
	
	
	
private:
	// ARMS 1p
	// cycle - reload - fireweapons press/release - aiming press-release
	void Input_CycleWeapon();
	void Input_ReloadWeapon();
	void Input_FireWeapon_Pressed();
	void Input_FireWeapon_Released();
	void Input_Aim_Pressed();
	void Input_Aim_Released();
	
	
	
	
	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> CycleWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> FireWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> ReloadWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> AimWeaponAction;
};
