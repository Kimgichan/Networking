// Fill out your copyright notice in the Description page of Project Settings.

#include "ByteLib.h"


void UByteLib::ByteLib_ByteToInt(UPARAM(ref) TArray<uint8> byte, int32 begin, int32& value) {
	value = *((int32*)(&(byte.GetData()[begin])));
}


void UByteLib::ByteLib_ByteToBool(UPARAM(ref)TArray<uint8> byte, int32 begin, bool& value)
{
	value = *((bool*)(&(byte.GetData()[begin])));
}

void UByteLib::ByteLib_ByteToFloat(UPARAM(ref)TArray<uint8> byte, int32 begin, float& value)
{
	value = *((float*)(&(byte.GetData()[begin])));
}

void UByteLib::ByteLib_ByteToString(UPARAM(ref)TArray<uint8> byte, int32 begin, int32 count, FString& value)
{
	TArray<uint8> reader;
	value.Empty(count);
	int32 max = begin + count;
	for (int32 i = begin; i < max; i++) {
		reader.Add(byte[i]);
	}
	value = FString((TCHAR*)reader.GetData());
}

void UByteLib::ByteLib_IntToByte(int32 value, TArray<uint8>& byteArray)
{
	uint8* reader = (uint8*)(&value);
	int32 max = sizeof(int32);
	for (int32 i = 0; i < max; i++) {
		byteArray.Add(reader[i]);
	}
}

void UByteLib::ByteLib_BoolToByte(bool value, TArray<uint8>& byteArray)
{
	uint8* reader = (uint8*)(&value);
	int32 max = sizeof(bool);
	for (int32 i = 0; i < max; i++) {
		byteArray.Add(reader[i]);
	}
}

void UByteLib::ByteLib_FloatToByte(float value, TArray<uint8>& byteArray)
{
	uint8* reader = (uint8*)(&value);
	int32 max = sizeof(value);
	for (int32 i = 0; i < max; i++) {
		byteArray.Add(reader[i]);
	}
}

void UByteLib::ByteLib_StringToByte(UPARAM(ref)FString value, int32 count, TArray<uint8>& byteArray)
{
	uint8* reader = (uint8*)value.GetCharArray().GetData();
	for (int32 i = 0; i < count; i++) {
		byteArray.Add(reader[i]);
	}
}
