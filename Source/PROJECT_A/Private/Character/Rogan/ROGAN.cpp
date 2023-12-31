// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Rogan/ROGAN.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MotionWarpingComponent.h"
#include "..\..\..\Public\GlobalUtilty.h"
#include "Camera/CameraComponent.h"
#include "Character/Animation/ROGANAnimInstance.h"
#include "Character/Component/ClimbComponent.h"
#include "Character/Component/CombatComponent.h"
#include "Character/Component/TargetLockComponent.h"
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

	if(bIsSprint && GetVelocity().Size2D() > 600.f)
	{
		SprintTime+=DeltaTime;
	}
	else
	{
		SprintTime = 0.f;
	}

	PrintEditorMessage(0.f,FString::Printf(TEXT("Sprint Time : %.2f"),SprintTime));
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
		EnhancedInputComponent->BindAction(AttackAction,ETriggerEvent::Triggered,this,&AROGAN::Attack);
		EnhancedInputComponent->BindAction(TargetLockAction,ETriggerEvent::Triggered,this,&AROGAN::TargetLock);
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
	if(Controller != nullptr && !AnimInstance->IsAnyMontagePlaying())
	{
		if(bIsWalk)
		{
			ForwardInputValue = MovementVector.Y;
			RightInputValue = MovementVector.X;
			MovementState = ECharacterMovementState::ECMS_Walk;
		}
		else if(bIsSprint)
		{
			ForwardInputValue = MovementVector.Y*3.f;
			RightInputValue = MovementVector.X*3.f;
			MovementState = ECharacterMovementState::ECMS_Sprint;
		}
		else
		{
			ForwardInputValue = MovementVector.Y*2.f;
			RightInputValue = MovementVector.X*2.f;
			MovementState = ECharacterMovementState::ECMS_Walk;
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
	if(ClimbComponent != nullptr)
	{
		ResetMovementValue();
		ClimbComponent->Climb();
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

void AROGAN::Attack()
{
	if(CombatComponent)
	{
		// TODO Dash Attack 추가
		if(SprintTime > 0.1f)
		{
			CombatComponent->Attack("DashAttack");
		}
		CombatComponent->Attack("Fighter");
	}
}

void AROGAN::TargetLock()
{
	if(TargetLockComponent)
	{
		TargetLockComponent->OnTargetLock.ExecuteIfBound();
	}
}

void AROGAN::TakeDamage(const float Damage, const FVector& Normal, FHitResult const& HitInfo, const float PushValue,
                        AActor* DamageCauser, TSubclassOf<UCameraShakeBase> CameraShakeBase)
{
	Super::TakeDamage(Damage, Normal, HitInfo, PushValue, DamageCauser, CameraShakeBase);
}

void AROGAN::EndDamaged()
{
	
}

void AROGAN::MoveToTarget(AActor* Target)
{
	
}

void AROGAN::MoveToTargets(TArray<AActor*> Targets)
{
	if(Targets.Num() > 0 )
	{
		AActor* Target = Targets[FMath::RandRange(0, Targets.Num() - 1)];
		if(Target)
		{
			const FVector TargetLocation = Target->GetActorLocation();
			const FVector ActorLocation = GetActorLocation();
			const FVector ForwardVector = UKismetMathLibrary::FindLookAtRotation(TargetLocation,ActorLocation).Vector();
			const FVector TargetForwardLocation = TargetLocation+(ForwardVector*40.f);
			const FVector WarpLocation = FVector(TargetForwardLocation.X,TargetForwardLocation.Y,GetActorLocation().Z);
			FRotator LookTargetRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation,TargetLocation);
			LookTargetRot.Roll = 0;
			LookTargetRot.Pitch = 0;

			//GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			FMotionWarpingTarget WarpingTarget;
			WarpingTarget.Name = FName("MeleeAttack");
			WarpingTarget.Location = WarpLocation;
			WarpingTarget.Rotation = LookTargetRot;
		
			MotionWarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
		}
	}
	else
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("MeleeAttack"));
	}
}
