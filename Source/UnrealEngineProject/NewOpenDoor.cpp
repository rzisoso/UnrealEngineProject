// Fill out your copyright notice in the Description page of Project Settings.


#include "NewOpenDoor.h"
#include <GameFramework/Actor.h>
#include <Components/PrimitiveComponent.h>
//#include <Kismet/GameplayStatics.h>
#include <UObject/ConstructorHelpers.h>
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UNewOpenDoor::UNewOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USoundCue> sound(TEXT("SoundCue'/Game/Lock_Cue.Lock_Cue'"));
	if (sound.Succeeded())
	{
		Sound = sound.Object;
	}
	Owner = GetOwner();
	LastOpenTime = 0.f;
	OpenTime = 1.f;

	
	
}


// Called when the game starts
void UNewOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DefaultPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	/*Trigger = Cast<ATriggerVolume>(UGameplayStatics::GetActorOfClass(this, ATriggerVolume::StaticClass()));
	UE_LOG(LogTemp, Warning, TEXT("Name: %s"), Trigger->tag);*/
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(this, TEXT("Trigger01"), Actors);
	if (Actors.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger Found!!!"));
		Trigger = Cast<ATriggerVolume>(Actors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger Not Found!!!"));
	}
	
	//OpenDoor();

	
}


// Called every frame
void UNewOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TArray<AActor*> array;
	float weight = 0.f;
	if (Trigger != nullptr)
	{
		Trigger->GetOverlappingActors(OUT array);
		for (auto &x : array)
		{
			weight += x->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
		
		if (weight >= 15)
		{
			
			
			OpenDoor();
			LastOpenTime = GetWorld()->GetTimeSeconds();
		}
		
		
		//UE_LOG(LogTemp, Warning, TEXT("Hello %s in OpenDoor!!!"), *DefaultPawn->GetName());
	}
	if (OpenTime < GetWorld()->GetTimeSeconds() - LastOpenTime)
	{
		CloseDoor();
	}
}

void UNewOpenDoor::OpenDoor()
{
	//Owner->SetActorRelativeRotation(FRotator(0, -180, 0));
	openDoor.Broadcast();
	//if (Sound)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT(" %f !!!"), weight);
	//	UGameplayStatics::PlaySound2D(this, Sound);
	//}
}

void UNewOpenDoor::CloseDoor()
{
	//Owner->SetActorRelativeRotation(FRotator(0, -90, 0));
	closeDoor.Broadcast();
	
}

