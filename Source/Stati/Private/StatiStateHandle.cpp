#include "StatiStateHandle.h"

FStatiStateHandle::FStatiStateHandle()
{
}

FStatiStateHandle::~FStatiStateHandle()
{
}

bool FStatiStateHandle::IsValid() const
{
	return Handle != 0;
}

void FStatiStateHandle::Invalidate()
{
	Handle = 0;
}

bool FStatiStateHandle::operator==(const FStatiStateHandle& Other) const
{
	return Handle == Other.Handle;
}

bool FStatiStateHandle::operator!=(const FStatiStateHandle& Other) const
{
	return Handle != Other.Handle;
}

FName FStatiStateHandle::GetName() const
{
	return Name;
}

FStatiStateHandle FStatiStateHandle::GenerateHandle(FName Name)
{
	uint64 NewSerialNumber = ++LastAssignedSerialNumber;
	
	if (!ensureMsgf(NewSerialNumber != FStatiStateHandle::MaxSerialNumber, TEXT("State serial number has wrapped around!")))
	{
		NewSerialNumber = (uint64)1;
	}

	FStatiStateHandle Result;

	Result.Handle = NewSerialNumber;

	return Result;
}

uint64 FStatiStateHandle::GetSerialNumber() const
{
	return Handle;
}

uint32 GetTypeHash(const FStatiStateHandle& InHandle)
{
	return GetTypeHash(InHandle.Handle);
}

#if WITH_EDITOR
FString FStatiStateHandle::ToString() const
{
	TStringBuilder<128> StringBuilder;

	StringBuilder.Append(FString::Printf(TEXT("%llu"), Handle));

	if (Name != NAME_None)
	{
		StringBuilder.Append(Name.ToString());
	}
	
	return StringBuilder.ToString();
}
#endif