// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"

/**
 * 
UCLASS()
class ZOMBIEGAME_API UActorHelper : public UObject
{
	GENERATED_BODY()
	
};
 */


namespace ActorHelper
{
	template <class ObjectType>
	auto FindObject(const FString& a_obj) -> ObjectType*
	{
		return ConstructorHelpers::FObjectFinder<ObjectType>(*a_obj).Object;
	}

    template <typename T>
    class TTickHelper
    {
        public:
        double LastTime = 0.0;
        double DeltaTime = 0.0;
        double CurrentTime = 0.0;

        TTickHelper() = default;

        // Nur erlauben, wenn T ein UObject ist (damit StaticClass() existiert)
        static FString Get()
        {
            static_assert(TIsDerivedFrom<T, UObject>::IsDerived,
                "T must derive from UObject for StaticClass()");
            return T::StaticClass()->GetName(); // Gibt FString zurück
        }

        // Tick und gib DeltaTime in Sekunden zurück
        double TickLog()
        {
            CurrentTime = FPlatformTime::Seconds();        // monotone Uhr
            DeltaTime = CurrentTime - LastTime;
            LastTime = CurrentTime;
            return DeltaTime;
        }
    };
	
}
