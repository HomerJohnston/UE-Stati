#pragma once

#include "StatiSystem.h"
#include "StatiState.generated.h"

class UStatiSystem;

UENUM(BlueprintType)
enum class EStatiStatePriority : uint8
{
	// (0) Do not use. This is for the managing subsystem to spawn default states (i.e. during normal gameplay when no overrides are set).
	__DefaultInternalOnly	UMETA(Hidden),

	// (1) Use for minimum priority state overrides. This could be for a level to set its own default state like making the player character act relaxed in a primarily casual level. 
	Background,

	// (2) Generic normal priority state for gameplay.
	Normal,

	// (3) Generic high priority state for gameplay or other higher things.
	High,

	// (4) Maximum priority state. Use for cutscenes or similar.
	CutScene
};

USTRUCT(BlueprintType)
struct STATI_API FStatiState
{
	GENERATED_BODY()

	// ============================================================================================
	// CONSTRUCTION
	// ============================================================================================
	
public:
	FStatiState();

	FStatiState(FName InStateName);
	
	FStatiState(EStatiStatePriority InPriority);
	
	FStatiState(FName InStateName, EStatiStatePriority InPriority);

	// ============================================================================================
	// STATE
	// ============================================================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(Transient)
	EStatiStatePriority Priority;
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = StatiSubstateData, ExcludeBaseStruct))
	TArray<FInstancedStruct> SystemData;

	UPROPERTY(Transient)
	TArray<UStatiSystem*> Systems;

	UPROPERTY(Transient)
	double CreationTime;
};
