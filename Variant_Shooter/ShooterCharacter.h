// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActorHelper.h"
#include "ZombieGameCharacter.h"
#include "ShooterWeaponHolder.h"
#include "ShooterCharacter.generated.h"

class AShooterWeapon;
class UInputAction;
class UInputComponent;
class UPawnNoiseEmitterComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBulletCountUpdatedDelegate, int32, MagazineSize, int32, Bullets);

/**
 *  A player controllable first person shooter character
 *  Manages a weapon inventory through the IShooterWeaponHolder interface
 *  Manages health and death
 */

UENUM(BlueprintType)
enum class EGait : uint8
{
	Walk   UMETA(DisplayName = "Walk"),
	Run    UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle   UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
};

UENUM(BlueprintType)
enum class EStances : uint8
{
	Stand   UMETA(DisplayName = "Stand"),
	Crouch UMETA(DisplayName = "Crouch"),
};

UENUM(BlueprintType)
enum class EStateMaschineStates : uint8
{
	IdleLoop					UMETA(DisplayName = "IdleLoop"),
	TransitionToIdleLoop		UMETA(DisplayName = "TransitionToIdleLoop"),
	LocomotionLoop				UMETA(DisplayName = "LocomotionLoop"),
	TransitionToLocomotionLoop	UMETA(DisplayName = "TransitionToLocomotionLoop"),
	InAirLoop					UMETA(DisplayName = "InAirLoop"),
	TranstionToInAirLoop		UMETA(DisplayName = "TranstionToInAirLoop"),
	IdleBreak					UMETA(DisplayName = "IdleBreak"),
};


UCLASS(abstract)
class ZOMBIEGAME_API AShooterCharacter : public AZombieGameCharacter, public IShooterWeaponHolder
{
	GENERATED_BODY()
	
	/** AI Noise emitter component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;

public:

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;


	//ALPHA 1.2
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	// UCameraComponent* ThirdPersonCameraComponent; 

protected:

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* FireAction;

	/** Fire weapon input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> Reload;

	/** Switch weapon input action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* SwitchWeaponAction;

	/** Max distance to use for aim traces */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Aim")
	float MaxAimDistance = 10000.0f;

	/** Current HP remaining to this character */
	UPROPERTY(EditAnywhere, Category="Health")
	float CurrentHP = 500.0f;

	/** Team ID for this character*/
	UPROPERTY(EditAnywhere, Category="Team")
	uint8 TeamByte = 0;

	/** List of weapons picked up by the character */
	TArray<AShooterWeapon*> OwnedWeapons;

	/** Weapon currently equipped and ready to shoot with */
	TObjectPtr<AShooterWeapon> CurrentWeapon;


	//ANIMATION STIZZLE

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Gait")
	float walkSpeed = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Gait")
	float runSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Gait")
	float sprintSpeed = 600.f;

	static EGait CalcGait(const AShooterCharacter* Char, float WalkMax, float RunMax, float SprintMin)
	{
		if (!Char) return EGait::Walk;
		const float speedXY = Char->GetVelocity().Size2D();

		if (speedXY >= SprintMin)           return EGait::Sprint;
		if (speedXY >= RunMax)              return EGait::Run;
		return EGait::Walk;
	};

	// EGAIT

	UFUNCTION(BlueprintPure, Category = "Movement|Gait")
	EGait GetGait() const { return CalcGait(ref, walkSpeed, runSpeed, sprintSpeed); };

	// ESTANCE

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Stance", meta = (AllowPrivateAccess = "true"))
	EStances eStances = EStances::Stand;

	UFUNCTION(BlueprintPure, Category = "Movement|Stance")
	EStances GetEStances() const { return eStances; };

	//EMovmentMode

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|Mode", meta = (AllowPrivateAccess = "true"))
	EMovementState eMovementState = EMovementState::Idle;

	UFUNCTION(BlueprintPure, Category = "Movement|Mode")
	EMovementState GetEMovmentState() const { return eMovementState; };

	// EStateMaschineStates

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement|StateMaschine", meta = (AllowPrivateAccess = "true"))
	EStateMaschineStates eStateMaschineStates = EStateMaschineStates::TransitionToIdleLoop;

	UFUNCTION(BlueprintPure, Category = "Movement|StateMaschine")
	EStateMaschineStates GetEStateMaschineState() const { return eStateMaschineStates; };

	// SHooterCharacter Reference

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerRef", meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* ref;

	UFUNCTION(BlueprintPure, Category = "PlayerRef")
	AShooterCharacter* GetReference() const { return ref; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bWantsToAim = false;

public:

	/** Bullet count updated delegate */
	FBulletCountUpdatedDelegate OnBulletCountUpdated;

	

public:

	/** Constructor */
	AShooterCharacter();


protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** ANIMATION FUNKTIONS **/
	void OnStartMovement()
	{
		bIsMoving = true;
	}

	void OnStopMovement()
	{
		bIsMoving = false;

	}

public:

	/** Handle incoming damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	/** Handles start firing input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStartFiring();

	/** Handles stop firing input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoStopFiring();

	/** Handles switch weapon input */
	UFUNCTION(BlueprintCallable, Category="Input")
	void DoSwitchWeapon();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoReload();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartAim();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStopAim();
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float IsAlive() const { return CurrentHP; }

public:

	//~Begin IShooterWeaponHolder interface

	/** Attaches a weapon's meshes to the owner */
	virtual void AttachWeaponMeshes(AShooterWeapon* Weapon) override;

	/** Plays the firing montage for the weapon */
	virtual void PlayFiringMontage(UAnimMontage* Montage) override;

	/** Applies weapon recoil to the owner */
	virtual void AddWeaponRecoil(float Recoil) override;

	/** Updates the weapon's HUD with the current ammo count */
	virtual void UpdateWeaponHUD(int32 CurrentAmmo, int32 MagazineSize) override;

	/** Calculates and returns the aim location for the weapon */
	virtual FVector GetWeaponTargetLocation() override;

	/** Gives a weapon of this class to the owner */
	virtual void AddWeaponClass(const TSubclassOf<AShooterWeapon>& WeaponClass) override;

	/** Activates the passed weapon */
	virtual void OnWeaponActivated(AShooterWeapon* Weapon) override;

	/** Deactivates the passed weapon */
	virtual void OnWeaponDeactivated(AShooterWeapon* Weapon) override;

	/** Notifies the owner that the weapon cooldown has expired and it's ready to shoot again */
	virtual void OnSemiWeaponRefire() override;


	//~End IShooterWeaponHolder interface

protected:

	/** Returns true if the character already owns a weapon of the given class */
	AShooterWeapon* FindWeaponOfType(TSubclassOf<AShooterWeapon> WeaponClass) const;

public:

	UCameraComponent* GetFirstPersonBodyMesh() const { return FirstPersonCameraComponent; }


};