// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

template<typename T>
static FString GetEnumDisplayNameToString(T EnumValue)
{
	FString EnumString = FString::Printf(TEXT("%hs"),typeid(T).name());
	EnumString.RemoveFromStart(TEXT("enum "));
	const TCHAR* Name = *EnumString;
	const UEnum* EnumPtr = FindFirstObjectSafe<UEnum>(Name);
	if (EnumPtr == nullptr)
	{
		return FString("Invalid");
	}
	return EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(EnumValue)).ToString();
}

