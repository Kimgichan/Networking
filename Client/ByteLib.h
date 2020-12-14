// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ByteLib.generated.h"

/**
 * 
 */
UCLASS()
class SOCKET_TEST_API UByteLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//byte->int
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_ByteToInt(UPARAM(ref) TArray<uint8> byte, int32 begin, int32& value);

	//byte->bool
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_ByteToBool(UPARAM(ref) TArray<uint8> byte, int32 begin, bool& value);

	//byte->float
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_ByteToFloat(UPARAM(ref) TArray<uint8> byte, int32 begin, float& value);

	//byte->string
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_ByteToString(UPARAM(ref) TArray<uint8> byte, int32 begin, int32 count, FString& value);

	//

	//int->byte
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_IntToByte(int32 value, TArray<uint8>& byteArray);

	//bool->byte
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_BoolToByte(bool value, TArray<uint8>& byteArray);

	//float->byte
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_FloatToByte(float value, TArray<uint8>& byteArray);

	//string->byte
	UFUNCTION(BlueprintCallable, Category = "ByteFunc")
		static void ByteLib_StringToByte(UPARAM(ref) FString value, int32 count,TArray<uint8>& byteArray);
};
