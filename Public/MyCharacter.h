// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.generated.h"

class USoundBase;
class UAnimMontage;
class UInputComponent;
class USceneComponent;
class UCameraComponent;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UCharacterMovementComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config = Game)
class ZOMBIEGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

};
