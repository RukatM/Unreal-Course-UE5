// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretComponent.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurretComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


// Called every frame
void UTurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimeSinceShot += DeltaTime;


	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	AActor* Owner = GetOwner();



	if (PlayerPawn && Owner)
	{
		FVector ToPlayer = CalculateRoationVector(PlayerPawn->GetActorLocation(), Owner->GetActorLocation());

		if (ToPlayer.SquaredLength() < FMath::Square(DetectionRange))
		{
			RotateTowards(ToPlayer, DeltaTime);
			if (CanFire() && DetectPlayerPawn(Owner, PlayerPawn))
			{
				Fire();
			}
		}
	}
}

FVector UTurretComponent::CalculateRoationVector(FVector TargetPosition, FVector SourcePosition)
{
	FVector RotationVector = TargetPosition - SourcePosition;
	RotationVector.Z = 0.f;
	return RotationVector;
}

void  UTurretComponent::RotateTowards(FVector RotationDirection, float DeltaTime)
{
	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromYZ(RotationDirection, FVector::UpVector);
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, RotateSpeed);
	GetOwner()->SetActorRotation(NewRotation);
}

bool UTurretComponent::DetectPlayerPawn(AActor* Detector, APawn* Target)
{
	FVector Start = Detector->GetActorLocation();
	Start.Z += 120;
	FVector End = Start + Detector->GetActorRightVector() * DetectionRange;

	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Detector);

	bool Hit = GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Pawn, Params);
	//Debug line
	/*DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 0.f, 0, 5.0f);*/
	if (Hit)
	{
		return Result.GetActor() == Target;
	}
	return false;
}

bool UTurretComponent::CanFire() const
{
	return ProjectileClass && TimeSinceShot >= 1.f/FireRate;
}

void UTurretComponent::Fire()
{
	TimeSinceShot = 0.f;
	AActor* Owner = GetOwner();
	FVector ProjectileSpawnLocation = Owner->GetActorLocation() + FVector(0.f, 0.f, 140.f) + Owner->GetActorRightVector() * 200.f;
	FRotator ProjectileSpawnRotation = Owner->GetActorRightVector().Rotation();

	FActorSpawnParameters ParamsProjectile;
	ParamsProjectile.Owner = Owner;
	ParamsProjectile.Instigator = Owner->GetInstigator();
	ParamsProjectile.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, ProjectileSpawnLocation, ProjectileSpawnRotation, ParamsProjectile);
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this->GetWorld(),
			FireSound,
			ProjectileSpawnLocation,
			1.0f,
			1.0f
		);
	}
}