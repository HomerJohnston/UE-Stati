#pragma once

#include "StatiState.h"
#include "StatiHandle.h"
#include "StatiManager.generated.h"

class UStatiSystem;
struct FStatiHandle;
struct FStatiState;

// ------------------------------------------------------------------------------------------------

UENUM(BlueprintType)
enum class EStatiManagerStatePriority : uint8
{
	PreferNewest,
	PreferExisting,
};

// ------------------------------------------------------------------------------------------------

USTRUCT()
struct FStatiHandleStateMap
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	TMap<FStatiHandle, FStatiState> States;

public:
	TDelegate<void(FStatiHandle, const FStatiState&)> OnStateAdded;
	
	TDelegate<void(FStatiHandle)> OnStateRemoved;
	
public:
	FStatiState& Add(FStatiHandle Handle, FStatiState State);

	int32 Remove(FStatiHandle Handle);

	FStatiState* Find(FStatiHandle Handle);
};

// ------------------------------------------------------------------------------------------------
#pragma region Tick Function
USTRUCT()
struct FStatiManagerTickFunction : public FTickFunction
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<class UStatiManager> Target;

	STATI_API void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

	STATI_API FString DiagnosticMessage() override;

	FName DiagnosticContext(bool bDetailed) override;
};

template<>
struct TStructOpsTypeTraits<FStatiManagerTickFunction> : public TStructOpsTypeTraitsBase2<FStatiManagerTickFunction>
{
	enum { WithCopy = false };
};
#pragma endregion
// ------------------------------------------------------------------------------------------------

UCLASS()
class STATI_API UStatiManager : public UWorldSubsystem
{
	GENERATED_BODY()

	// ============================================================================================
	// CONSTRUCTION
	// ============================================================================================

public:
	UStatiManager();

	// ============================================================================================
	// STATE
	// ============================================================================================
	
private:
	UPROPERTY()
	FStatiManagerTickFunction TickFunction;

protected:
	UPROPERTY(Transient)
	TMap<FName, FStatiHandle> NamedStateHandles;
	
	UPROPERTY(Transient)
	FStatiHandleStateMap States;

	UPROPERTY(Transient)
	FStatiHandle ActiveHandle;

	UPROPERTY(Transient)
	FStatiState ActiveState;
	
	UPROPERTY(Transient)
	TMap<TSubclassOf<UStatiSystem>, UStatiSystem*> Systems;

	UPROPERTY(Transient)
	bool bStateSelectionDirty = false;
	
	// ------------------------------------------
	// State Getters/Setters
	// ------------------------------------------

	// ------------------------------------------
	// Delegates/Events
	// ------------------------------------------

	// ============================================================================================
	// METHODS
	// ============================================================================================

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	void SetupDefaultStates();
	
	void SpawnSystemInstances();

public:
	UFUNCTION(BlueprintCallable)
	bool HasNamedStateHandle(FName HandleName);
	
	UFUNCTION(BlueprintCallable)
	FStatiHandle PushState(UPARAM(ref) const FStatiState& NewState, EStatiManagerStatePriority Priority = EStatiManagerStatePriority::PreferNewest);

	UFUNCTION(BlueprintCallable)
	bool PopState(UPARAM(ref) const FStatiHandle& StateHandle);

protected:
	void OnStateAdded(FStatiHandle NewHandle, const FStatiState& NewState);
	
	void OnStateRemoved(FStatiHandle RemovedHandle);
	
	void SetActive(FStatiHandle NewHandle, const FStatiState& NewState);
	
public:
	void Tick(float DeltaTime);

protected:
	void UpdateActiveStates(float DeltaTime);

	void TickSystems(float DeltaTime);
	
	// ============================================================================================
	// EDITOR STATE
	// ============================================================================================

	// ------------------------------------------
	// Editor State Getters/Setters
	// ------------------------------------------

	// ============================================================================================
	// EDITOR METHODS
	// ============================================================================================
	
};