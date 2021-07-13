// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/TriggerVolume.h>
#include "NewOpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALENGINEPROJECT_API UNewOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewOpenDoor();
private:
	AActor* Owner;
	//UPROPERTY(EditAnywhere)
	ATriggerVolume* Trigger;
	APawn* DefaultPawn;
	class USoundCue* Sound;
	float LastOpenTime;
	float OpenTime;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor();
	void CloseDoor();
	UPROPERTY(BlueprintAssignable)
	FDoorEvent openDoor;
	UPROPERTY(BlueprintAssignable)
	FDoorEvent closeDoor;
};
