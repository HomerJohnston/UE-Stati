#pragma once

#include "StatiState.generated.h"

UENUM(BlueprintType)
enum class EStatiStatePriority : uint8
{
	Low,
	Normal,
	High,
	VeryHigh,
	Maximum
};

USTRUCT(BlueprintType)
struct FStatiState
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	EStatiStatePriority Priority = EStatiStatePriority::Normal;

	UPROPERTY(Transient)
	FName Name;
	
	UPROPERTY(Transient)
	TMap<TSubclassOf<UStatiSubstate>, UStatiSubstate*> Substates;
};
