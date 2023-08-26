#include "StatiManager.h"

#include "StatiState.h"
#include "StatiStateHandle.h"

void FStatiHandleStateMap::Add(FStatiStateHandle Handle, FStatiState State)
{
	States.Add(Handle, State);
	
	OnStatesChanged.Execute();
}

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

void UStatiManager::Initialize(FSubsystemCollectionBase& Collection)
{
	States.OnStatesChanged.BindRaw(this, &ThisClass::OnStatesChanged);
}

bool UStatiManager::HasNamedStateHandle(FName HandleName)
{
	return NamedStateHandles.Contains(HandleName);
}

FStatiStateHandle UStatiManager::PushState(const FStatiState& NewState, EStatiManagerStatePriority Priority, bool bOverrideExistingNamedHandles)
{
	if (NewState.Name != NAME_None)
	{
		FStatiStateHandle* ExistingHandle = NamedStateHandles.Find(NewState.Name);

		if (ExistingHandle)
		{
			if (bOverrideExistingNamedHandles)
			{
				States.Add(*ExistingHandle, NewState);
			}

			return *ExistingHandle;
		}
	}
	
	FStatiStateHandle NewStateHandle = FStatiStateHandle::GenerateHandle(NewState.Name);

	if (NewState.Name != NAME_None)
	{
		NamedStateHandles.Add(NewState.Name, NewStateHandle);
	}

	States.Add(NewStateHandle, NewState);
	
	return NewStateHandle;
}

bool UStatiManager::PopState(const FStatiStateHandle& StateHandle)
{
	return false;
}

void UStatiManager::Tick(float DeltaTime)
{
	UpdateActiveStates(DeltaTime);
}

void UStatiManager::OnStatesChanged()
{
	// I can either sort and reprocess the states right now, or I can queue a state sort for the end of the frame to save doing it more than once?
	bStatesDirty = true;
}

void UStatiManager::UpdateActiveStates(float DeltaTime)
{
	if (!bStatesDirty)
	{
		return;
	}
}
