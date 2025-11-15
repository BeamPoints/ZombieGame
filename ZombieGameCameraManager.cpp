// Copyright Epic Games, Inc. All Rights Reserved.


#include "ZombieGameCameraManager.h"

AZombieGameCameraManager::AZombieGameCameraManager()
{
	// set the min/max pitch
	ViewPitchMin = -70.0f;
	ViewPitchMax = 80.0f;
	ViewTargetOffset = FVector(0.0f, 0.0f, 90.f);
}
