// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideDoorOpener.h"

// Sets default values for this component's properties
USlideDoorOpener::USlideDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	// ...
}


// Called when the game starts
void USlideDoorOpener::BeginPlay()
{
	Super::BeginPlay();
	
	if (DoorTimeline && DoorCurve)
	{
		SetUpTimeline();
	}
	// ...
	
}


// Called every frame
void USlideDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// ...

}

void USlideDoorOpener::SetUpTimeline()
{
	FOnTimelineFloat SlideProgress;
	SlideProgress.BindUFunction(this,FName("HandleTimelineProgress"));
	DoorTimeline->AddInterpFloat(DoorCurve,SlideProgress);
}

void USlideDoorOpener::InitializeDoor(UStaticMeshComponent* Left, UStaticMeshComponent* Right, UBoxComponent* BoxCollision)
{
	if (Left && Right && BoxCollision)
	{
		m_Left = Left;
		m_Right = Right;
		m_BoxCollision = BoxCollision;

		LeftStartPosition = m_Left->GetComponentLocation();
		RightStartPosition = m_Right->GetComponentLocation();

		m_BoxCollision->OnComponentBeginOverlap.AddDynamic(this,&USlideDoorOpener::OverlapBegin);
		m_BoxCollision->OnComponentEndOverlap.AddDynamic(this,&USlideDoorOpener::OverlapEnd);
		UE_LOG(LogTemp, Warning, TEXT("Left Position %s"),*LeftStartPosition.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Right Position %s"),*RightStartPosition.ToString());
	}
	else 
		UE_LOG(LogTemp, Warning, TEXT("Attatch Static mesh to func"));
}

void USlideDoorOpener::Open()
{
	UE_LOG(LogTemp, Warning, TEXT("Opening"));
	IsOpen = true;
	DoorTimeline->Play();
	if (DoorSound)
	{
		PlaySoundEffect();
	}
}

void USlideDoorOpener::Close()
{
	UE_LOG(LogTemp, Warning, TEXT("CLosing"));
	IsOpen = false;
	DoorTimeline->Reverse();
	if (DoorSound)
	{
		PlaySoundEffect();
	}
}

void USlideDoorOpener::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0) && !IsOpen)
	{
		Open();
	}
}

void USlideDoorOpener::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0) && IsOpen)
	{
		Close();
	}
}

void USlideDoorOpener::HandleTimelineProgress(const float Value)
{
	const FVector LSlide = FVector(-SlideDistance,0,0);
	const FVector RSlide = FVector(SlideDistance,0,0);
	
	m_Left->SetWorldLocation(FMath::Lerp(LeftStartPosition,LeftStartPosition + LSlide,Value));
	m_Right->SetWorldLocation(FMath::Lerp(RightStartPosition,RightStartPosition + RSlide,Value));
}

void USlideDoorOpener::PlaySoundEffect()
{
	UGameplayStatics::PlaySoundAtLocation(
			this->GetWorld(),
			DoorSound,
			GetOwner()->GetActorLocation(),
			1.0f,
			1.0f
		);
}

