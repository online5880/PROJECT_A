// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/GameplayStatics.h"

/**
 * @brief EnemyObjectChannel 정의
 */
#define EnemyObjectType UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1)
/**
 * @brief 타입(T)를 Enum 을 FString 으로 반환하는 함수
 * @tparam T 타입
 * @param EnumValue Enum
 * @return Enum 을 FString 으로 반환
 */
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

/**
 * @brief 에디터상에서 메시지 출력 (AddOnScreenDebugMessage)
 * @param Time 메시지 시간
 * @param Message 메시지
 */
static void PrintEditorMessage(const float Time,const FString& Message)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			Time,
			FColor::Orange,
			Message);	
	}
}

/**
 * @brief 2D Sound 재생 (class SoundBase)
 * @param World UWorld
 * @param Sound USoundBase
 */
static void PlaySound2D(const UWorld* World,USoundBase* Sound)
{
	checkf(World,TEXT("World is nullptr"));
	checkf(Sound,TEXT("Sound is nullptr"));
	
	UGameplayStatics::PlaySound2D(
		World,
		Sound);
}

