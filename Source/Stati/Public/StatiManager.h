#pragma once

#include "StatiManager.generated.h"

struct FStatiStateHandle;
struct FStatiState;
class UStatiSubstate;

UENUM(BlueprintType)
enum class EStatiManagerStatePriority : uint8
{
	PreferNewest,
	PreferExisting,
};

USTRUCT()
struct FStatiHandleStateMap
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Transient)
	TMap<FStatiStateHandle, FStatiState> States;

public:
	FSimpleDelegate OnStatesChanged;
	
public:
	void Add(FStatiStateHandle Handle, FStatiState State);
};

USTRUCT()
struct FStatiManagerTickFunction : public FTickFunction
{
	GENERATED_BODY()

public:
	class TWeakObjectPtr<UStatiManager> Target;

	STATI_API void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

	STATI_API FString DiagnosticMessage() override;

	FName DiagnosticContext(bool bDetailed) override;
};

template<>
struct TStructOpsTypeTraits<FStatiManagerTickFunction> : public TStructOpsTypeTraitsBase2<FStatiManagerTickFunction>
{
	enum { WithCopy = false };
};

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
	TMap<FName, FStatiStateHandle> NamedStateHandles;
	
	UPROPERTY(Transient)
	FStatiHandleStateMap States;

	UPROPERTY(Transient)
	TMap<TSubclassOf<UStatiSubstate>, UStatiSubstate*> ActiveSubstates;

	UPROPERTY(Transient)
	bool bStatesDirty = false;
	
	// -------------------------------------------------------------------
	// State Getters/Setters
	// -------------------------------------------------------------------

	// -------------------------------------------------------------------
	// Delegates/Events
	// -------------------------------------------------------------------

	// ============================================================================================
	// METHODS
	// ============================================================================================

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	bool HasNamedStateHandle(FName HandleName);
	
	UFUNCTION(BlueprintCallable)
	FStatiStateHandle PushState(UPARAM(ref) const FStatiState& NewState, EStatiManagerStatePriority Priority = EStatiManagerStatePriority::PreferNewest, bool bOverrideExistingNamedHandles = true);

	UFUNCTION(BlueprintCallable)
	bool PopState(UPARAM(ref) const FStatiStateHandle& StateHandle);

public:
	void Tick(float DeltaTime);

protected:
	void OnStatesChanged();

	void UpdateActiveStates(float DeltaTime);
	
	// ============================================================================================
	// EDITOR STATE
	// ============================================================================================

	// -------------------------------------------------------------------
	// Editor State Getters/Setters
	// -------------------------------------------------------------------

	// ============================================================================================
	// EDITOR METHODS
	// ============================================================================================
	
};