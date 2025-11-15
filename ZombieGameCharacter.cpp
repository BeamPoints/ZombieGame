// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/LODSyncComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GroomComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// AZombieGameCharacter

AZombieGameCharacter::AZombieGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//LOADING DEFAULT FACE INTO Chractert for Default LODSYNC

	//USkeletalMesh* DefaultFaceMesh = LoadObject<USkeletalMesh>(nullptr,TEXT("/Game/MetaHumans/Common/Face/SKM_Face_Preview.SKM_Face_Preview")); <- DONT WORK
	// 
	// 
	// Create the first person mesh that will be viewed only by this character's owner
	// Create a SceneComponent to serve as the root for the character's body -> Body ist main mesh


	SceneComponentRootCharacter = CreateDefaultSubobject<USceneComponent>(TEXT("RootCharacter"));
	SceneComponentRootCharacter->SetupAttachment(GetCapsuleComponent());
	SceneComponentRootCharacter->SetRelativeRotation(DefaultRotator);

	LodSyncComponent = CreateDefaultSubobject<ULODSyncComponent>(TEXT("LodSyncComponent"));

	// SETUP BODY MESH
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(SceneComponentRootCharacter);
	FirstPersonMesh->AttachToComponent(SceneComponentRootCharacter, FAttachmentTransformRules::KeepRelativeTransform);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));


	WeaponPosComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponPosComponent"));
	WeaponPosComponent->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform, GetFirstPersonWeaponSocket());
	//GunSocket = FirstPersonMesh->GetSocketByName(FName("GunSocket")); // Ensure the socket exists for weapon attachment
	//----------------
	// SETUP FEET MESH
	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FEET"));
	FeetMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform);
	FeetMesh->SetCollisionProfileName(FName("NoCollision"));
	//----------------
	// SETUP LEG MESH
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LEG"));
	LegMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LegMesh->SetCollisionProfileName(FName("NoCollision"));
	//----------------
	// SETUP TORSO MESH
	TorsoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TORSO"));
	TorsoMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TorsoMesh->SetCollisionProfileName(FName("NoCollision"));
	//----------------
	// SETUP FACE MESH
	FaceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FACE"));
	FaceMesh->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::KeepRelativeTransform);
	FaceMesh->SetCollisionProfileName(FName("NoCollision"));
	//----------------
	// SETUP FACE HAIR MESHES

	Mustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
	Mustache->AttachToComponent(FaceMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("FACIAL_C_LipUpper"));

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->AttachToComponent(FaceMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("FACIAL_C_FacialRoot"));

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->AttachToComponent(FaceMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("FACIAL_C_FacialRoot"));

	Beard = CreateDefaultSubobject<UGroomComponent>(TEXT("Beard"));
	Beard->AttachToComponent(FaceMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("Facial_C_JAW"));

	// LOD SYNC SETUP
	// DEFINDE LIKE -> FLODMappingData(int32 InMasterLODIndex, int32 InChildLODIndex);
	// INIT LOD SETTINGS

	if (LodSyncComponent && Mustache && Hair && Beard && Eyebrows)
	{
		LodSyncComponent->ComponentsToSync.Add(FComponentSync(Mustache->GetFName(), ESyncOption::Drive));
		LodSyncComponent->ComponentsToSync.Add(FComponentSync(Eyebrows->GetFName(), ESyncOption::Drive));
		LodSyncComponent->ComponentsToSync.Add(FComponentSync(Hair->GetFName(), ESyncOption::Drive));
		LodSyncComponent->ComponentsToSync.Add(FComponentSync(Beard->GetFName(), ESyncOption::Drive));

		// Set up custom LOD mapping
		LodSyncComponent->CustomLODMapping.Add(Mustache->GetFName());
		LodSyncComponent->CustomLODMapping.Add(Hair->GetFName());
		LodSyncComponent->CustomLODMapping.Add(Beard->GetFName());
		LodSyncComponent->CustomLODMapping.Add(Eyebrows->GetFName());

		LodSyncComponent->RefreshSyncComponents();
	}

	// Create the Camera Component	MOVED -> ShooterCharacter

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	
	
}
/////////////////////////////////// POST INIT ///////////////////////////////////////
void AZombieGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Ensure the LOD Sync component is initialized after all components are created
	if (LodSyncComponent)
	{
		LodSyncComponent->InitializeComponent();
	}
	// SET MASTER POSE FOR ALL BODY PARTS
	FeetMesh->SetLeaderPoseComponent(FirstPersonMesh);
	LegMesh->SetLeaderPoseComponent(FirstPersonMesh);
	TorsoMesh->SetLeaderPoseComponent(FirstPersonMesh);

}
/////////////////////////////////// POST INIT END ///////////////////////////////////