// Fill out your copyright notice in the Description page of Project Settings.
		

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameFramework/Character.h"
#include "ShooterTypes.h"
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
		
		// fixing aim rotation up and down.
		UFUNCTION(BlueprintCallable)
		FRotator GetFixedAimRotation() const;

		UPROPERTY(BlueprintReadOnly, Category = "FPS_FABRIK")
		FTransform FABRIK_SocketTransform;
	
	
		UFUNCTION(BlueprintPure, Category = "FPS_Combat")
		bool HasCurrentWeapons() const;
	

protected:	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "FPS_Combat");
    	TObjectPtr<UCombatComponent> Combat;

		UFUNCTION(BlueprintImplementableEvent)
		void OnAim(bool bIsAiming);
	
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS_Aiming")
		float DefaultFieldOfView;
	
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "FPS_Camera")
		TObjectPtr<UCameraComponent> FirstPersonCamera;
	
		UPROPERTY(BlueprintReadOnly, Category= "FPS_TurnInPlace")
    	float AO_Yaw;
	
		UPROPERTY(BlueprintReadOnly, Category= "FPS_Strafing")
		float MovementOffsetYaw;
	
		UPROPERTY(BlueprintReadOnly, Category = "FPS_TurnInPlace")
		ETurningInPlace TurningStatus;
	
	
	
	
private:
	// ARMS 1p
	// cycle - reload - fireweapons press/release - aiming press-release
	void Input_CycleWeapon();
	void Input_ReloadWeapon();
	void Input_FireWeapon_Pressed();
	void Input_FireWeapon_Released();
	void Input_Aim_Pressed();
	void Input_Aim_Released();
	
	void CalculateFABRIKSocketTransform();
	
	void CalcuteTurnInPlaceParameters(float DeltaTime);
	
	void TurnInPlace(float DeltaTime);
	
	
	FRotator StartingAimRotation;
	float InterpAO_Yaw;
	
	
	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> CycleWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> FireWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> ReloadWeaponAction;
	
	UPROPERTY(EditAnywhere, Category= "FPS_Input");
	TObjectPtr<UInputAction> AimWeaponAction;
};
