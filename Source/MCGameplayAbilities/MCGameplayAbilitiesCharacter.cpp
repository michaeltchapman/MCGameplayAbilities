// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MCGameplayAbilitiesCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MCAttributeSet.h"
#include "MCAbilitySystemComponent.h"
#include "MCCharacterAttributeSet.h"

//////////////////////////////////////////////////////////////////////////
// AMCGameplayAbilitiesCharacter

AMCGameplayAbilitiesCharacter::AMCGameplayAbilitiesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystem = CreateDefaultSubobject<UMCAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UMCCharacterAttributeSet>(TEXT("AttributeSet"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMCGameplayAbilitiesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMCGameplayAbilitiesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMCGameplayAbilitiesCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMCGameplayAbilitiesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMCGameplayAbilitiesCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMCGameplayAbilitiesCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMCGameplayAbilitiesCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Confirm", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::ConfirmAbility);

	PlayerInputComponent->BindAction("AbilitySlot0", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot0);
	PlayerInputComponent->BindAction("AbilitySlot1", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot1);
	PlayerInputComponent->BindAction("AbilitySlot2", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot2);
	PlayerInputComponent->BindAction("AbilitySlot3", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot3);
	PlayerInputComponent->BindAction("AbilitySlot4", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot4);
	PlayerInputComponent->BindAction("AbilitySlot5", IE_Pressed, this, &AMCGameplayAbilitiesCharacter::AbilitySlot5);
}

UAbilitySystemComponent* AMCGameplayAbilitiesCharacter::GetAbilitySystemComponent() const
{
	return Cast<UAbilitySystemComponent>(AbilitySystem);
}

void AMCGameplayAbilitiesCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMCGameplayAbilitiesCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMCGameplayAbilitiesCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMCGameplayAbilitiesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMCGameplayAbilitiesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMCGameplayAbilitiesCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMCGameplayAbilitiesCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMCGameplayAbilitiesCharacter::ConfirmAbility()
{
	AbilitySystem->TargetConfirm();
}

void AMCGameplayAbilitiesCharacter::ActivateAbilityInSlot(int32 Slot)
{
	if (AbilitySystem)
	{
		AbilitySystem->TryActivateAbilityByClass(AbilitySystem->ActiveSlots[Slot]);
	}
}

void AMCGameplayAbilitiesCharacter::AbilitySlot0()
{
	ActivateAbilityInSlot(0);
}

void AMCGameplayAbilitiesCharacter::AbilitySlot1()
{
	ActivateAbilityInSlot(1);
}

void AMCGameplayAbilitiesCharacter::AbilitySlot2()
{
	ActivateAbilityInSlot(2);
}

void AMCGameplayAbilitiesCharacter::AbilitySlot3()
{
	ActivateAbilityInSlot(3);
}

void AMCGameplayAbilitiesCharacter::AbilitySlot4()
{
	ActivateAbilityInSlot(4);
}

void AMCGameplayAbilitiesCharacter::AbilitySlot5()
{
	ActivateAbilityInSlot(5);
}
