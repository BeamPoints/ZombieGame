// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UGroomComponent;
class ULODSyncComponent;
struct FInputActionValue;


/**
 *  A basic first person character
 */
UCLASS(abstract)
class AZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()


public:

	FRotator3d DefaultRotator = FRotator3d(0,-90,0);
	bool bIsMoving = false;


	/// Basic Face Hair Groom Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Mustache;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Eyebrows;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Hair;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGroomComponent> Beard;

	//Character Meshes

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponentRootCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FeetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* LegMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* TorsoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FaceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
	USceneComponent* WeaponPosComponent;

	// The Camera Socket is used to attach the camera to the character's face mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSockets", meta = (AllowPrivateAccess = "true"))
	 FName CameraSocket = FName("CameraSocket");

	// The Gun Socket is used to attach the weapon to the character's Body Mesh or FirstPersonMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSockets", meta = (AllowPrivateAccess = "true"))
	 FName GunSocket = FName("GunSocket");

	 UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSockets", meta = (AllowPrivateAccess = "true"))
	 ULODSyncComponent* LodSyncComponent;

	// FiRST PERSON CAMERA COMPONENT -> Moved -> ShooterCharacter
	// INPUT BASIC -> MOVED -> ShooterCharacter

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* MouseLookAction;

	// Aim Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MouseActionAim;


	
public:
	AZombieGameCharacter();

	virtual void PostInitializeComponents() override;

	static void LinkPose(USkeletalMeshComponent* Slave, USkeletalMeshComponent* Master)
	{
		if (!Master || !Slave) return;
		else
		{
			Slave->SetLeaderPoseComponent(Master);
		}
	}

public:

	/** Returns the first person mesh **/ // NO FIRST PERSON MESH JUST BASE OF Character -> NEW CHARACTER MODEL FROM METAHUMAN
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* GetFaceMesh() const { return FaceMesh; }

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* GetFirstPersonBodyMesh() const { return FirstPersonMesh; }

	UFUNCTION(BlueprintCallable, Category = "Sockets")
	FName GetFirstPersonWeaponSocket() const { return GunSocket; }

	UFUNCTION(BlueprintCallable, Category = "Sockets")
	FName GetCameraSocket() const { return CameraSocket; }

	UFUNCTION(BlueprintCallable, Category = "Input/Animations")
	bool GetPlayerMovementState() const { return bIsMoving; }
};

