#include "StatiManager.h"

#include "StatiState.h"
#include "StatiHandle.h"
#include "StatiSystem.h"

// ------------------------------------------------------------------------------------------------
FStatiState& FStatiHandleStateMap::Add(FStatiHandle Handle, FStatiState State)
{
	FStatiState& StoredState = States.Add(Handle, State);
	
	OnStateAdded.Execute(Handle, State);

	return StoredState;
}

// ------------------------------------------------------------------------------------------------
int32 FStatiHandleStateMap::Remove(FStatiHandle Handle)
{
	return States.Remove(Handle);
}

// ------------------------------------------------------------------------------------------------
FStatiState* FStatiHandleStateMap::Find(FStatiHandle Handle)
{
	return States.Find(Handle);
}

// ------------------------------------------------------------------------------------------------
#pragma region Tick Function
void FStatiManagerTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	if (Target.IsValid() && TickType != LEVELTICK_ViewportsOnly)
	{
		Target->Tick(DeltaTime);
	}
}

FString FStatiManagerTickFunction::DiagnosticMessage()
{
	return TEXT("StatiManagerTickFunction");
}

FName FStatiManagerTickFunction::DiagnosticContext(bool bDetailed)
{
	return FName("StatiManager");
}

UStatiManager::UStatiManager()
{
}
#pragma endregion 
// ------------------------------------------------------------------------------------------------

void UStatiManager::Initialize(FSubsystemCollectionBase& Collection)
{
	States.OnStateAdded.BindUObject(this, &ThisClass::OnStateAdded);

	SpawnSystemInstances();
}

void UStatiManager::SetupDefaultStates()
{
	
}

// ------------------------------------------------------------------------------------------------
void UStatiManager::SpawnSystemInstances()
{
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;

		if (!Class->IsChildOf(UStatiSystem::StaticClass()))
		{
			continue;
		}

		UStatiSystem* SystemInstance = NewObject<UStatiSystem>(this, Class);
		
		Systems.Add(Class, SystemInstance);
	}

	Systems.Compact();
}

// ------------------------------------------------------------------------------------------------
bool UStatiManager::HasNamedStateHandle(FName HandleName)
{
	return NamedStateHandles.Contains(HandleName);
}

// ------------------------------------------------------------------------------------------------
FStatiHandle UStatiManager::PushState(const FStatiState& NewState, EStatiManagerStatePriority Priority)
{		
	if (NewState.Name != NAME_None)
	{
		FStatiHandle* ExistingHandle = NamedStateHandles.Find(NewState.Name);

		if (ExistingHandle)
		{
			FStatiState& StoredState = States.Add(*ExistingHandle, NewState);
			StoredState.CreationTime = GetWorld()->GetTimeSeconds();
			
			return *ExistingHandle;
		}
	}
	
	FStatiHandle NewStateHandle = FStatiHandle::GenerateHandle(NewState.Name);

	if (NewState.Name != NAME_None)
	{
		NamedStateHandles.Add(NewState.Name, NewStateHandle);
	}

	FStatiState& StoredState = States.Add(NewStateHandle, NewState); // This will naturally call into OnStatesChanged
	StoredState.CreationTime = GetWorld()->GetTimeSeconds();

	return NewStateHandle;
}

// ------------------------------------------------------------------------------------------------
bool UStatiManager::PopState(const FStatiHandle& StateHandle)
{
	return (States.Remove(StateHandle) > 0);
}

// ------------------------------------------------------------------------------------------------
void UStatiManager::OnStateAdded(FStatiHandle NewHandle, const FStatiState& NewState)
{
	if (NewState.Priority > ActiveState.Priority)
	{
		SetActive(NewHandle, NewState);
	}
	else
	{
		// TODO I can either sort and reprocess the states right now, or I can queue a state sort for the end of the frame to save doing it more than once?
		// I'll try queuing it.
		bStateSelectionDirty = true;
	}
}

// ------------------------------------------------------------------------------------------------
void UStatiManager::OnStateRemoved(FStatiHandle RemovedHandle)
{
	if (RemovedHandle == ActiveHandle)
	{
		bStateSelectionDirty = true;
	}
}

// ------------------------------------------------------------------------------------------------
void UStatiManager::SetActive(FStatiHandle NewHandle, const FStatiState& NewState)
{
	ActiveHandle = NewHandle;

	ActiveState = NewState;
}

// ------------------------------------------------------------------------------------------------
void UStatiManager::Tick(float DeltaTime)
{
	if (bStateSelectionDirty)
	{
		UpdateActiveStates(DeltaTime);
	}

	TickSystems(DeltaTime);
}

// -------------------------------------------------------------------------------------------------
void UStatiManager::UpdateActiveStates(float DeltaTime)
{
	// For every system, I need to find the most prominent state data. I do not expect users to stack even dozens of states (probably always single digits), so I can afford to do something naive.
	// I could build an array of state handles because they're small/cheap to move around a lot, and sort them based on their associated state's priority.
	// After that, I find each state from highest priority to lowest. I then iterate through each states' data and update each System to their new states.

	// TODO I am here

	bStateSelectionDirty = false;
}

void UStatiManager::TickSystems(float DeltaTime)
{
	for (auto [Class, System] : Systems)
	{
		System->Tick(DeltaTime);
	}
}
