// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARInteractionComponent.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "GameFramework\CharacterMovementComponent.h"

// Sets default values
AARCharacter::AARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARInteractionComponent>("InteractionComponent");

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("AttributeComponent");

	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComponent");

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AARCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AARCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARCharacter::MoveRight);


	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AARCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AARCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &AARCharacter::Ability);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AARCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AARCharacter::SprintStop);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARCharacter::PrimaryInteract);
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void AARCharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
                                   float Delta)
{
	if(Delta < 0)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		AttributeComp->AddRage(InstigatorActor, Delta); 
	}

	
	if(NewHealth <= 0 && Delta < 0)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f);
	}
}

void AARCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AARCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AARCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AARCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "Blackhole");
}

void AARCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AARCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AARCharacter::Ability()
{
	ActionComp->StartActionByName(this, "Dash");
}

void AARCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}