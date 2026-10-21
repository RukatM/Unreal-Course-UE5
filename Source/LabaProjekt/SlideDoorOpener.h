// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
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
	UFUNCTION(BlueprintCallable, Category = "Door")
		void InitializeDoor(UStaticMeshComponent* Left, UStaticMeshComponent* Right, UBoxComponent* BoxCollision);

	UPROPERTY(EditAnywhere, Category = "Slide Door")
	float AbsOffset = 380.f;

private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> m_Left = nullptr;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> m_Right = nullptr;

	UPROPERTY()
	TObjectPtr<UBoxComponent> m_BoxCollision = nullptr;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector LeftStartPosition = FVector::Zero();
	FVector RightStartPosition = FVector::Zero();

	void Open();
	void Close();
	bool IsOpen = false;
};
