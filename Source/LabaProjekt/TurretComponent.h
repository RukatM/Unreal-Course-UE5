// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"

#include "TurretComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LABAPROJEKT_API UTurretComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTurretComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Turret")
	float RotateSpeed = 120.f;
	UPROPERTY(EditAnywhere, Category = "Turret")
	float DetectionRange = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Turret")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Turret")
	USoundBase* FireSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<AActor> ProjectileClass;

private:
	FVector CalculateRoationVector(FVector TargetPosition, FVector SourcePosition);
	void  RotateTowards(FVector RotationDirection, float DeltaTime);
	bool DetectPlayerPawn(AActor* Detector, APawn* Target);
	bool CanFire() const;
	void Fire();
	float TimeSinceShot = 0.f;

};
