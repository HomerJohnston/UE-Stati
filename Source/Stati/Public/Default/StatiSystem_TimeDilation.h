#pragma once

#include "StatiSystem.h"

#include "StatiSystem_TimeDilation.generated.h"

// ------------------------------------------------------------------------------------------------
USTRUCT()
struct STATI_API FStatiSystem_TimeDilation_StateData : public FStatiSystemStateData
{
	GENERATED_BODY()

	// ============================================================================================
	// CONSTRUCTION
	// ============================================================================================
	
public:
	FStatiSystem_TimeDilation_StateData();

	// ============================================================================================
	// SETTINGS
	// ============================================================================================
	
protected:
	/** The time dilation value to use for this state. */
	UPROPERTY(EditAnywhere, DisplayName = "Activated Dilation", BlueprintReadWrite)
	float Value = 1.0;

	/** Whether or not to use a start transition time. */
	UPROPERTY(BlueprintReadWrite)
	bool bUseStartTransitionTime = false;

	/** Fade to the dilation value over this long, in game seconds (affected by time dilation). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Activating Transition Time", meta=(UIMin=0.0, UIMax=3.0, EditCondition = "bUseStartTransitionTime"))
	float StartTransitionTime = 0.0;

	/** Whether or not to use an end transition time. */
	UPROPERTY(BlueprintReadWrite)
	bool bUseEndTransitionTime = false;

	/** Fade out of this dilation value over this long, in game seconds (affected by time dilation). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Deactivating Transition Time", meta=(UIMin=0.0, UIMax=3.0, EditCondition = "bUseEndTransitionTime"))
	float EndTransitionTime = 0.0;

	
	// ============================================================================================
	// STATE
	// ============================================================================================
	
	// ============================================================================================
	// API
	// ============================================================================================

};

// ------------------------------------------------------------------------------------------------
UCLASS()
class STATI_API UStatiSystem_TimeDilation : public UStatiSystem
{
	GENERATED_BODY()
	
	// ============================================================================================
	// CONSTRUCTION
	// ============================================================================================

public:
	UStatiSystem_TimeDilation();
	
	// ============================================================================================
	// SETTINGS
	// ============================================================================================
	
	
	// ============================================================================================
	// STATE
	// ============================================================================================

protected:
	UPROPERTY(Transient)
	FStatiSystem_TimeDilation_StateData CurrentStateData;
};