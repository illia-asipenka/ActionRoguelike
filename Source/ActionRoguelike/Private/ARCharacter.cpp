// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"

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

	bUseControllerRotationYaw = false;
	AttackDelay = 0.2f;
	AbilityDelay = 0.4f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AARCharacter::PrimaryInteract);
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
	PlayAnimMontage(PrimaryAttackMontage);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AARCharacter::PrimaryAttack_TimeElapsed, AttackDelay);
}

void AARCharacter::SecondaryAttack()
{
	PlayAnimMontage(SecondaryAttackMontage);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AARCharacter::SecondaryAttack_TimeElapsed, AttackDelay);
}

void AARCharacter::Ability()
{
	PlayAnimMontage(AbilityMontage);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AARCharacter::Ability_TimeElapsed, AttackDelay);
}

void AARCharacter::SecondaryAttack_TimeElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	const FVector ProjectileSpawnLocation = HandLocation + GetActorForwardVector() * 20.0f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	const FTransform SpawnTM = FTransform(GetControlRotation(), ProjectileSpawnLocation);

	GetWorld()->SpawnActor<AActor>(WarpProjectileClass, SpawnTM, SpawnParams);
}


void AARCharacter::PrimaryAttack_TimeElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector ProjectileSpawnLocation = HandLocation + GetActorForwardVector() * 20.0f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams::AllObjects;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComp->GetForwardVector()*1000000;
	bool bIsHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams);
	UE_LOG(LogTemp, Warning, TEXT("Hit result: %s"), bIsHit ? TEXT("True") : TEXT("False"));

	FRotator ProjectileSpawnRotation = bIsHit ? (Hit.ImpactPoint - HandLocation).Rotation() : GetControlRotation();
	const FTransform SpawnTM = FTransform(ProjectileSpawnRotation, ProjectileSpawnLocation);

	GetWorld()->SpawnActor<AActor>(MagicProjectileClass, SpawnTM, SpawnParams);
}

void AARCharacter::Ability_TimeElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	const FVector ProjectileSpawnLocation = HandLocation + GetActorForwardVector() * 20.0f;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	const FTransform SpawnTM = FTransform(GetControlRotation(), ProjectileSpawnLocation);

	GetWorld()->SpawnActor<AActor>(AbilityProjectileClass, SpawnTM, SpawnParams);
}

void AARCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}
