#pragma once

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	ECMS_Idle UMETA(DisplayName = "Idle"),
	ECMS_Walk UMETA(DisplayName = "Walk"),
	ECMS_Jogging UMETA(DisplayName = "Jogging"),
	ECMS_Sprint UMETA(DisplayName = "Sprint"),
	ECMS_Falling UMETA(DisplayName = "Falling"),
	ECMS_Crouch UMETA(DisplayName = "Crouch")
};