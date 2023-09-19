// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryyActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)

// Sets default values
ABaseGeometryyActor::ABaseGeometryyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void ABaseGeometryyActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	
	//PrintTransform();
	//PrintStringTypes();
	//PrintTypes();

	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseGeometryyActor::OnTimerFired, GeometryData.TimerRate, true);
}

void ABaseGeometryyActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogBaseGeometry, Error, TEXT("Actor %s is dead."), *GetName());
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABaseGeometryyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandeMovement();
}

void ABaseGeometryyActor::PrintTypes()
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Weapons num: %d, kills num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %.2f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("IsDead: %d"), IsDead);
	UE_LOG(LogBaseGeometry, Warning, TEXT("HasWeapon: %d"), static_cast<int>(HasWeapon));
}

void ABaseGeometryyActor::PrintStringTypes()
{
	UE_LOG(LogBaseGeometry, Display, TEXT("Name: %s"), *Name)

	FString WeaponNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "health = " + FString::SanitizeFloat(Health);
	FString IsDeadStr = "isDead = " + FString(IsDead ? "True" : "False");

	FString stat = FString::Printf(TEXT("\n == All Stat ==\n %s\n %s\n %s"), *WeaponNumStr, *HealthStr, *IsDeadStr);
	UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *stat);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Name);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, stat, true, FVector2D(1.5f, 1.5f));
	}
}

void ABaseGeometryyActor::PrintTransform()
{
	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotation: %s"), *Rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale: %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Transform (human): %s"), *Transform.ToHumanReadableString());
}

void ABaseGeometryyActor::HandeMovement()
{
	switch (GeometryData.MoveType)
	{
	case EMovementType::Sin:
	{
		FVector CurrentLocation = GetActorLocation();
		if (GetWorld())
		{
			float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
			SetActorLocation(CurrentLocation);
		}
	}
	break;
	case EMovementType::Static: break;
	default: break;
	}
}

void ABaseGeometryyActor::SetColor(const FLinearColor& color)
{
	if (!BaseMesh) return;
	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", color);
	}
}

void ABaseGeometryyActor::OnTimerFired()
{
	if (++TimerCount <= MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseGeometry, Display, TEXT("Color to set up: %s"), *NewColor.ToString());
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else 
	{
		UE_LOG(LogBaseGeometry, Warning, TEXT("Timer hass been stopped!"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}

