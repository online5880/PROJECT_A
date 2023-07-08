// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/ROGAN.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/Animation/ROGANAnimInstance.h"
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
	
	Init();
}

void AROGAN::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SmoothCameraRotation(DeltaTime);
	CheckFalling();
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
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AROGAN::Dodge);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AROGAN::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AROGAN::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&AROGAN::StopJumping);
	}
}

void AROGAN::Init()
{
	if(const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}

	if(AnimInstance == nullptr)
	{
		AnimInstance = Cast<UROGANAnimInstance>(GetMesh()->GetAnimInstance());
	}
}

void AROGAN::SmoothCameraRotation(float DeltaTime)
{
	if(RightInputValue != 0.f || ForwardInputValue != 0.f)
	{
		const FRotator InterpRot = UKismetMathLibrary::RInterpTo(GetActorRotation(),GetControlRotation(),DeltaTime,6.f);
		SetActorRotation(FRotator(0.f,InterpRot.Yaw,0.f));
	}
}

void AROGAN::CheckFalling()
{
	bIsFalling = GetCharacterMovement()->IsFalling();
}

void AROGAN::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2d>();

	if(Controller != nullptr)
	{
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
	bIsWalk = false;
	bIsSprint = false;
}

void AROGAN::Walk(const FInputActionValue& Value)
{
	bIsWalk = Value.Get<bool>();	
}

void AROGAN::Sprint(const FInputActionValue& Value)
{
	bIsSprint = Value.Get<bool>();
	if(bIsCrouch)
	{
		UnCrouch();
		bIsCrouch =false;
	}
}

void AROGAN::Crouching(const FInputActionValue& Value)
{
	const bool CanCrouching = !bIsCrouch && !bIsFalling && !bIsSprint;
	
	if(CanCrouching)
	{
		bIsCrouch = true;
		Crouch();
	}
	else
	{
		const FVector Start = GetActorLocation();
		const FVector End = (GetActorUpVector()*100+Start);
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			TraceTypeQuery1,
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			HitResult,
			true);
		if(!HitResult.bBlockingHit)
		{
			
			bIsCrouch = false;
			UnCrouch();
		}
	}
}

void AROGAN::Dodge(const FInputActionValue& Value)
{
	if(RightInputValue != 0.f || ForwardInputValue != 0.f && !bIsDodge)
	{
		FTimerHandle DodgeTimer;
		bIsDodge = true;
		GetWorldTimerManager().SetTimer(DodgeTimer,[this]
		{
			bIsDodge = false;
		},1.f,false);
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
	/*if(!bIsFalling)
	{
		Super::Jump();
	}
	else
	{
		if(!bIsClimbing)
		{
			Climb();
		}
	}*/
	if(!bIsClimbing)
	{
		Climb();
	}
}

void AROGAN::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	if(AnimInstance)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}
}

void AROGAN::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if(AnimInstance)
	{
		AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);	
	}
}

void AROGAN::Climb()
{
	if(AnimInstance && Climb_Montage)
	{
		FHitResult ClimbHitResult_1;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
		UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			GetActorLocation(),
			GetActorForwardVector()*125.f+GetActorLocation(),
			ObjectTypes,
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::ForDuration,
			ClimbHitResult_1,
			true);
		
		
		/*AnimInstance->SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
		AnimInstance->Montage_Play(Climb_Montage);
		bIsClimbing = true;
		FOnMontageEnded ClimbEndedDelegate;
		ClimbEndedDelegate.BindLambda([this](UAnimMontage* Montage,bool bInterrupted)
		{
			bIsClimbing = false;
			UE_LOG(LogTemp,Error,TEXT("Climb End"));
		});
		AnimInstance->Montage_SetEndDelegate(ClimbEndedDelegate);*/
	}
}

