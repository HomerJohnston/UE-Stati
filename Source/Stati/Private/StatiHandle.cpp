#include "StatiHandle.h"

FStatiHandle::FStatiHandle()
{
}

FStatiHandle::~FStatiHandle()
{
}

bool FStatiHandle::IsValid() const
{
	return Handle != 0;
}

void FStatiHandle::Invalidate()
{
	Handle = 0;
}

bool FStatiHandle::operator==(const FStatiHandle& Other) const
{
	return Handle == Other.Handle;
}

bool FStatiHandle::operator!=(const FStatiHandle& Other) const
{
	return Handle != Other.Handle;
}

FName FStatiHandle::GetName() const
{
	return Name;
}

FStatiHandle FStatiHandle::GenerateHandle(FName Name)
{
	uint64 NewSerialNumber = ++LastAssignedSerialNumber;
	
	if (!ensureMsgf(NewSerialNumber != FStatiHandle::MaxSerialNumber, TEXT("State serial number has wrapped around!")))
	{
		NewSerialNumber = (uint64)1;
	}

	FStatiHandle Result;

	Result.Handle = NewSerialNumber;

	return Result;
}

uint64 FStatiHandle::GetSerialNumber() const
{
	return Handle;
}

uint32 GetTypeHash(const FStatiHandle& InHandle)
{
	return GetTypeHash(InHandle.Handle);
}

#if WITH_EDITOR
FString FStatiHandle::ToString() const
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