// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include <Components/StaticMeshComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/SphereComponent.h>
#include <DrawDebugHelpers.h>
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include <Components/PrimitiveComponent.h>

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//ConstructorHelpers::FObjectFinder<USphereComponent> Sphere(TEXT(""))
	SetRootComponent(Collision);
	Collision->SetSphereRadius(15);
	Collision->SetMassOverrideInKg(NAME_None, 60.0f);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	if (StaticMesh != nullptr && Mesh.Succeeded())
	{
		
		StaticMesh->SetStaticMesh(Mesh.Object);
	}
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//SpringArm->AttachTo(GetRootComponent());
	Camera->SetupAttachment(GetRootComponent());
	//SpringArm->TargetArmLength = -130.0f;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 100.0f));

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	Speed = 200.0f;
	Velocity = FVector(0.f);

	View = FVector(0.f);
	ViewRotator = FRotator(0.f);

	Hit = FHitResult();

	Origen = FRotator(0.f);
	
	
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Origen = GetActorRotation();

	AddActorLocalOffset(Velocity * DeltaTime, true);


	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(View, ViewRotator);
	DrawDebugLine(GetWorld(), View, View + ViewRotator.Vector() * 150, FColor(255, 0, 0), false, 0, 0, 10);

	GetWorld()->LineTraceSingleByObjectType(Hit, View, View + ViewRotator.Vector() * 150, FCollisionObjectQueryParams(ECC_PhysicsBody));
	Hit.GetComponent();
	if (Hit.GetActor() != nullptr)
	{

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());
	}

	if (PhysicsHandle->GetGrabbedComponent())
	{
		//float x = (View + ViewRotator.Vector).X;
		UE_LOG(LogTemp, Warning, TEXT("Location: %f, %f, %f"), (View + ViewRotator.Vector() * 10).X, (View + ViewRotator.Vector() * 10).Y, (View + ViewRotator.Vector() * 10).Z);
		PhysicsHandle->SetTargetLocation(View + ViewRotator.Vector() * 100);
		if (Origen != ViewRotator)
		{
			/*PhysicsHandle->SetTargetRotation(FRotator(PhysicsHandle->GetGrabbedComponent()->GetComponentRotation().Pitch, PhysicsHandle->GetGrabbedComponent()->GetComponentRotation().Yaw + ViewRotator.Yaw - Origen.Yaw, PhysicsHandle->GetGrabbedComponent()->GetComponentRotation().Roll));*/
			UE_LOG(LogTemp, Warning, TEXT("Rotator: %f, %f, %f"), Com.Pitch, Com.Yaw + ViewRotator.Yaw - Origen.Yaw, Com.Roll);
			PhysicsHandle->SetTargetRotation(FRotator(Com.Pitch, Com.Yaw + ViewRotator.Yaw - Origen.Yaw, Com.Roll));
		}
		
	}
	
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyPawn::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AMyPawn::LookRight);
	PlayerInputComponent->BindAction(TEXT("Grab"), EInputEvent::IE_Pressed, this, &AMyPawn::Grab);
	PlayerInputComponent->BindAction(TEXT("Grab"), EInputEvent::IE_Released, this, &AMyPawn::UnGrab);
}

void AMyPawn::MoveForward(float n)
{
	Velocity.X = FMath::Clamp(n, -1.f, 1.f) * Speed;
	
}



void AMyPawn::MoveRight(float n)
{
	Velocity.Y = FMath::Clamp(n, -1.f, 1.f) * Speed;
}



void AMyPawn::LookUp(float n)
{
	FRotator oldLook = Camera->GetComponentRotation();
	FRotator newLook = FRotator(FMath::Clamp(oldLook.Pitch + n, -20.f, 60.f), oldLook.Yaw, oldLook.Roll);
	Camera->SetWorldRotation(newLook);
	//UE_LOG(LogTemp, Warning, TEXT("%f!!!"), newLook.Pitch);
	//FRotator()
}

void AMyPawn::LookRight(float n)
{
	AddControllerYawInput(n);
}

void AMyPawn::Grab()
{
	if (Hit.GetActor() && PhysicsHandle)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(StaticCast<UPrimitiveComponent*>( Hit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass())), NAME_None, Hit.GetActor()->GetActorLocation(), Hit.GetActor()->GetActorRotation());
		Origen = GetActorRotation();
		Com = Hit.GetActor()->GetActorRotation();
		/*PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()), NAME_None, FVector(0.f), Hit.GetActor()->GetActorRotation());*/
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("wrong!!!!!!!!!!!!"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Grab!!!"));
}

void AMyPawn::UnGrab()
{
	if (PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp, Warning, TEXT("UnGrab!!!"));
}

