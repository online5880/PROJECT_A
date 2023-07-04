// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/ROGAN.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

AROGAN::AROGAN()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
}

void AROGAN::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}
}

void AROGAN::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(RightInputValue != 0.f || ForwardInputValue != 0.f)
	{
		const FRotator InterpRot = UKismetMathLibrary::RInterpTo(GetActorRotation(),GetControlRotation(),DeltaTime,6.f);
		SetActorRotation(FRotator(0.f,InterpRot.Yaw,0.f));
	}

}

void AROGAN::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&Jump);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AROGAN::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AROGAN::ResetMovementValue);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AROGAN::Walk);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AROGAN::Sprint);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AROGAN::Crouching);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AROGAN::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AROGAN::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AROGAN::StopJumping);
	}
}

void AROGAN::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2d>();

	if(Controller != nullptr)
	{
		/*const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);*/
		if(bIsWalk)
		{
			ForwardInputValue = MovementVector.Y;
			RightInputValue = MovementVector.X;
		}
		else if(bIsSprint)
		{
			ForwardInputValue = MovementVector.Y*3.f;
			RightInputValue = MovementVector.X*3.f;
		}
		else
		{
			ForwardInputValue = MovementVector.Y*2.f;
			RightInputValue = MovementVector.X*2.f;
		}	
	}
}

void AROGAN::ResetMovementValue()
{
	ForwardInputValue = 0.f;
	RightInputValue = 0.f;
}

void AROGAN::Walk(const FInputActionValue& Value)
{
	bIsWalk = Value.Get<bool>();	
}

void AROGAN::Sprint(const FInputActionValue& Value)
{
	bIsSprint = Value.Get<bool>();
}

void AROGAN::Crouching(const FInputActionValue& Value)
{
	if(!bIsCrouch)
	{
		bIsFalling = true;
		bIsCrouch = true;
	}
	else
	{
		bIsFalling = false;
		bIsCrouch = false;
	}
}

void AROGAN::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AROGAN::Jump()
{
	if(!bIsFalling)
	{
		Super::Jump();	
	}
}

void AROGAN::StopJumping()
{
	Super::StopJumping();
}

