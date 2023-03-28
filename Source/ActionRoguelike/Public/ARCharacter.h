// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UARInteractionComponent;
class UAnimMontage;
class UARAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARAttributeComponent* AttributeComp;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> WarpProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> AbilityProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* PrimaryAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* SecondaryAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AbilityMontage;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,class UARAttributeComponent* OwningComp, float NewHealth, float Delta);

	FTimerHandle AttackTimerHandle;
	float AttackDelay;
	float AbilityDelay;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void PrimaryAttack();
	void SecondaryAttack();
	void Ability();
	void SecondaryAttack_TimeElapsed();
	void PrimaryAttack_TimeElapsed();
	void Ability_TimeElapsed();
	void PrimaryInteract();

	virtual void PostInitializeComponents() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	AARCharacter();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};
