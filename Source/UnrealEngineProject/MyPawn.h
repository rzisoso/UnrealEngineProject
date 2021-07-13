// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class UNREALENGINEPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	/*UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;*/
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere)
	class UPhysicsHandleComponent* PhysicsHandle;
	
	float Speed;
	FVector Velocity;

	FVector View;
	FRotator ViewRotator;

	FRotator Origen;

	FRotator Com;

	FHitResult Hit;

	void MoveForward(float n);
	
	void MoveRight(float n);
	
	void LookUp(float n);
	void LookRight(float n);

	void Grab();
	void UnGrab();
};
