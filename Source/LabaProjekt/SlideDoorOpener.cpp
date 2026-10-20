// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideDoorOpener.h"

// Sets default values for this component's properties
USlideDoorOpener::USlideDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USlideDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USlideDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	AActor* Owner = GetOwner();

	if (PlayerPawn && Owner)
	{
		FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - Owner->GetActorLocation();

		if (DirectionToPlayer.SquaredLength() < FMath::Square(DetectionRange))
		{
			UE_LOG(LogTemp, Warning, TEXT("Detected"));
			if(!IsOpen) Open();
		}
	}

	// ...

}

void USlideDoorOpener::Open()
{
	FVector Offset(0.f,0.f,0.f);
	if (DoorType == EDoorType::Left) Offset.X = AbsOffset;
	else Offset.X = -1 * AbsOffset;
	FVector TargetLocation = GetOwner()->GetActorLocation() + Offset;
	GetOwner()->SetActorLocation(TargetLocation, false);
}

