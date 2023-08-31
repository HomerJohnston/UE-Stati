#include "StatiState.h"

FStatiState::FStatiState() :
	Priority(EStatiStatePriority::Normal)
{
}

FStatiState::FStatiState(FName InStateName) :
	Name(InStateName),
	Priority(EStatiStatePriority::Normal)
{
}

FStatiState::FStatiState(EStatiStatePriority InPriority) :
	Priority(InPriority)
{	
}

FStatiState::FStatiState(FName InStateName, EStatiStatePriority InPriority) :
	Name(InStateName),
	Priority(InPriority)
{
}
