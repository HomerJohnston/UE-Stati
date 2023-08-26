#pragma once

#include "StatiStateHandle.generated.h"

/** Unique handle that can be used to distinguish timers that have identical delegates. */
USTRUCT(BlueprintType)
struct STATI_API FStatiStateHandle
{
	GENERATED_BODY()

	//friend struct FTimerHeapOrder; // TODO go see what this thing did in the timer manager system

	// ============================================================================================
	// CONSTRUCTION
	// ============================================================================================

public:
	FStatiStateHandle();

	~FStatiStateHandle();

	// ============================================================================================
	// STATE
	// ============================================================================================
	
private:
	UPROPERTY(Transient)
	uint64 Handle = 0;

	UPROPERTY(Transient)
	FName Name = NAME_None;
	
	// TODO: What happens when I start doing save/loads? I will need to serialize this and start off where it left off or there could be conflicts.
	inline static uint64 LastAssignedSerialNumber = 0;
	
	// ============================================================================================
	// API
	// ============================================================================================
	
public:
	/** True if this handle was ever initialized by the timer manager */
	bool IsValid() const;

	/** Explicitly clear handle */
	void Invalidate();

	bool operator==(const FStatiStateHandle& Other) const;

	bool operator!=(const FStatiStateHandle& Other) const;
	
	FName GetName() const;

	static FStatiStateHandle GenerateHandle(FName Name = NAME_None);

private:
	static const uint32 IndexBits        = 24;

	static const uint32 SerialNumberBits = 40;

	static_assert(IndexBits + SerialNumberBits == 64, "The space for the state index and serial number should total 64 bits");

	static const int32  MaxIndex        = (int32)1 << IndexBits;
	
	static const uint64 MaxSerialNumber = (uint64)1 << SerialNumberBits;

	uint64 GetSerialNumber() const;

private:
	friend uint32 GetTypeHash(const FStatiStateHandle& InHandle);

	// ============================================================================================
	// EDITOR
	// ============================================================================================
	
#if WITH_EDITOR
public:
	FString ToString() const;
#endif
};