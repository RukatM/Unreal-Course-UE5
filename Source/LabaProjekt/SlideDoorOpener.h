// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "SlideDoorOpener.generated.h"

UENUM(BlueprintType)
enum class EDoorType : uint8
{
	Left        UMETA(DisplayName = "Left Door"),
	Right     UMETA(DisplayName = "Right Door")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LABAPROJEKT_API USlideDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlideDoorOpener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Slide Door")
	EDoorType DoorType = EDoorType::Left;

	UPROPERTY(EditAnywhere, Category = "Slide Door")
	float DetectionRange = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Slide Door")
	float AbsOffset = 380.f;

private:
	void Open();
	bool IsOpen = false;

		
};
