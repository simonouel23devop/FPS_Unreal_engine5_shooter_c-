// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"



UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};



class FPS_API IPlayerInterface
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FName GetWeaponAttachPoint(const FGameplayTag& WeaponType)const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent * GetMesh1P() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent * GetMesh3P() const;
	
};
