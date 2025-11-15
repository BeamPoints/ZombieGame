// Fill out your copyright notice in the Description page of Project Settings.


#include "Variant_Shooter/ZombieGame_AnimInstance.h"

static TAutoConsoleVariable<int32>	DDCvar_MMDatabaseLOD					(TEXT("DDCvar.MMDatabaseLOD"),						0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);
static TAutoConsoleVariable<float>	DDCvar_OffsetRootBone_TranslationRadius	(TEXT("DDCvar.OffsetRootBone.TranslationRadius"),	0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);
static TAutoConsoleVariable<bool>	a_animnode_offsetrootbone_enable		(TEXT("a.animnode.offsetrootbone.enable"),			0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);
static TAutoConsoleVariable<bool>	DDCVar_ThreadSafeAnimationUpdate_Enable	(TEXT("DDCVar.ThreadSafeAnimationUpdate.Enable"),	0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);
static TAutoConsoleVariable<bool>	DDCVar_ExperimentalStateMachine_Debug	(TEXT("DDCVar.ExperimentalStateMachine.Debug"),		0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);
static TAutoConsoleVariable<bool>	DDCVar_ExperimentalStateMachine_Enable	(TEXT("DDCVar.ExperimentalStateMachine.Enable"),	0, TEXT("AnimationBP_ConsoleValue"), ECVF_Scalability);

UZombieGame_AnimInstance::UZombieGame_AnimInstance()
{
	bIsInAir = false;
	bIsAlive = false;
	currentSpeed = 0.0f;
	bCanRevived = false;
	Helper.TickLog();

	int32 LodGlobal = DDCvar_MMDatabaseLOD.GetValueOnAnyThread();
	float translationRadius = DDCvar_OffsetRootBone_TranslationRadius.GetValueOnAnyThread();
	bool bOffsetRootBoneEnable = a_animnode_offsetrootbone_enable.GetValueOnAnyThread();
	bool bThreadSafeAnimationUpdateEnable = DDCVar_ThreadSafeAnimationUpdate_Enable.GetValueOnAnyThread();
	bool bExperimentalStateMachineDebug = DDCVar_ExperimentalStateMachine_Debug.GetValueOnAnyThread();
	bool bExperimentalStateMachineEnable = DDCVar_ExperimentalStateMachine_Enable.GetValueOnAnyThread();
}

void UZombieGame_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerReference = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UZombieGame_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (PlayerReference)
	{
		UpdateAnimationProperties();
	}
	else
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterCharacter::StaticClass(), OutActors);
		
		for (AActor* Actor : OutActors)
		{
			PlayerReference = Cast<AShooterCharacter>(Actor);
			if (PlayerReference)
			{
				UpdateAnimationProperties();
				break;
			}
		}
	}
}

void UZombieGame_AnimInstance::UpdateAnimationProperties()
{
	CurrentPlayerLocation = PlayerReference->GetActorTransform();
	// Vergleiche die Location mit Toleranz, da FTransform keinen == Operator hat
	if (CurrentPlayerLocation.GetLocation().Equals(LastUpdatePlayerLocation.GetLocation(), KINDA_SMALL_NUMBER))
	{
		// ...
		UE_LOG(LogTemp, Warning, TEXT("NO UPDATE"));
		if (!PlayerReference->GetPlayerMovementState()|| bPerformanceBool)
		{
			bCanStrafe = !PlayerReference->GetCharacterMovement()->bOrientRotationToMovement;
			playerLocation = FVector3f(CurrentPlayerLocation.GetLocation());
			updateLoction = FVector3f(LastUpdatePlayerLocation.GetLocation());
			currentSpeed = PlayerReference->GetVelocity().Size2D() / PlayerReference->GetCharacterMovement()->GetMaxSpeed();
			Currentforward = GetForwardVector(PlayerReference->GetVelocity(), PlayerReference->GetActorRightVector());
			if (bCanStrafe)
			{
				CurrentStrave = GetStraveVector(PlayerReference->GetVelocity(), PlayerReference->GetActorForwardVector());
			}
			bIsInAir = PlayerReference->GetCharacterMovement()->IsFalling();
			bPerformanceBool = false;
		}
		else if(!bPerformanceBool)
		{
			bPerformanceBool = true;
		}
		
	}
	else
	{
		bCanStrafe = !PlayerReference->GetCharacterMovement()->bOrientRotationToMovement;
		currentSpeed = PlayerReference->GetVelocity().Size();
		Currentforward = GetForwardVector(PlayerReference->GetVelocity(), PlayerReference->GetActorRightVector());
		if (bCanStrafe)
		{
			CurrentStrave = GetStraveVector(PlayerReference->GetVelocity(), PlayerReference->GetActorForwardVector());
		}
		bIsInAir = PlayerReference->GetCharacterMovement()->IsFalling();
		if (PlayerReference->IsAlive() == 0.5f)
		{
			bCanRevived = true;
		}
		else
		{
			bCanRevived = false;
		}
		if (PlayerReference->IsAlive() >= 0.0f)
		{
			bIsAlive = true;
		}
		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("-----ANIM UPDATE-----"));
			if (GetMovementComp() != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Max Speed: %f"), GetMovementComp()->GetMaxSpeed());
			}
			UE_LOG(LogTemp, Warning, TEXT("Current Speed: %f"), currentSpeed);
			UE_LOG(LogTemp, Warning, TEXT("Current Forward float: %f"), Currentforward);
			UE_LOG(LogTemp, Warning, TEXT("Current Strave float: %f"), CurrentStrave);
			UE_LOG(LogTemp, Warning, TEXT("CurrPlayerLoc %s"), *CurrentPlayerLocation.GetLocation().ToString());
			UE_LOG(LogTemp, Warning, TEXT("LastPlayerLoc %s"), *LastUpdatePlayerLocation.GetLocation().ToString());
		}
		LastUpdatePlayerLocation = CurrentPlayerLocation;
	}
	const double tickTime = Helper.TickLog();
	UE_LOG(LogTemp, Display, TEXT("UPDATE Time %f"), tickTime);
}

float UZombieGame_AnimInstance::GetStraveVector(FVector velocetyVector, FVector forwardVector)
{
	return FVector::DotProduct(forwardVector, velocetyVector.GetSafeNormal());
}

float UZombieGame_AnimInstance::GetForwardVector(FVector velocetyVector, FVector rightVector)
{
	return FVector::DotProduct(rightVector, velocetyVector.GetSafeNormal());
}


