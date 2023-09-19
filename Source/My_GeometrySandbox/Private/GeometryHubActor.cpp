// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub, All, All)

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();
	
	//DoActorSpawn1();
	//DoActorSpawn2();
	DoActorSpawn3();
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeometryHubActor::DoActorSpawn1()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.f, 140.f + 400 * i, 400.f));
			ABaseGeometryyActor* Geometry = World->SpawnActor<ABaseGeometryyActor>(GeometryClass, GeometryTransform);

			if (Geometry)
			{
				FGeometryData Data;
				Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Geometry->SetGeometryData(Data);
				//UE_LOG(LogTemp, Warning, TEXT("Spawned actor #%i\n Transform %s"), i, *GeometryTransform.ToHumanReadableString());
			}
		}
	}
}

void AGeometryHubActor::DoActorSpawn2()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < 10; ++i)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.f, 140.f + 400 * i, 900.f));
			ABaseGeometryyActor* Geometry = World->SpawnActorDeferred<ABaseGeometryyActor>(GeometryClass, GeometryTransform);

			if (Geometry)
			{
				FGeometryData Data;
				//Data.MoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
				Data.Color = FLinearColor::MakeRandomColor();
				Geometry->SetGeometryData(Data);
				Geometry->FinishSpawning(GeometryTransform);
				//UE_LOG(LogTemp, Warning, TEXT("Spawned actor #%i\n Transform %s"), i, *GeometryTransform.ToHumanReadableString());
			}
		}
	}
}

void AGeometryHubActor::DoActorSpawn3()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (const auto& Payload : GeometryPayloads)
		{
			ABaseGeometryyActor* Geometry = World->SpawnActorDeferred<ABaseGeometryyActor>(Payload.GeometryClass, Payload.InitialTranform);

			if (Geometry)
			{
				Geometry->SetGeometryData(Payload.Data);
				Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
				Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubActor::OnTimerFinished);
				Geometry->FinishSpawning(Payload.InitialTranform);
			}
		}
	}
}

void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor name: %s || Color %s"), *Name, *Color.ToString());
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if (!Actor) return;
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor %s finished color changes!"), *GetName());

	ABaseGeometryyActor* Geometry = Cast<ABaseGeometryyActor>(Actor);
	if (!Geometry) return;
	UE_LOG(LogGeometryHub, Display, TEXT("Cast is succesfull, %s"), *Geometry->GetGeometryData().ToHumanReadableString());

	Geometry->Destroy();
	Geometry->SetLifeSpan(2.f); // Lifetime after call of Destroy
}

