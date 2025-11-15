// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorHelper.h"
#include "ShooterCharacter.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UZombieGame_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:
	UZombieGame_AnimInstance();

	protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	float GetStraveVector(FVector velocetyVector, FVector forwardVector);
	float GetForwardVector(FVector velocetyVector, FVector rightVector);
	bool bPerformanceBool = true;
	ActorHelper::TTickHelper<ACharacter> Helper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DEBUG")
	bool bDebug = true;
	//CANNOT BE MAPED TO BLUEPRINT DUE TO TRANSFORM3D TYPE
	FTransform3d LastUpdatePlayerLocation;
	FTransform3d CurrentPlayerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	FVector3f updateLoction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	FVector3f playerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class AShooterCharacter* PlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	float currentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	float CurrentStrave;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	float Currentforward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	bool bCanStrafe;



	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	bool bCanRevived;

	UFUNCTION(BlueprintCallable, Category = "Animations")
	virtual void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = "Input/Animations")
	UPawnMovementComponent* GetMovementComp() const {return PlayerReference ? PlayerReference->GetMovementComponent() : nullptr;}

	/// CONSOLE VARIABLE FOR ANIM INSTANCE

	//virtual bool ProcessConsoleVariables() override;



};
