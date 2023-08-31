#include "Default/StatiSystem_TimeDilation.h"

FStatiSystem_TimeDilation_StateData::FStatiSystem_TimeDilation_StateData()
{
	SubstateType = UStatiSystem_TimeDilation::StaticClass();
}

UStatiSystem_TimeDilation::UStatiSystem_TimeDilation()
{
	StructType = FStatiSystem_TimeDilation_StateData::StaticStruct();
}
