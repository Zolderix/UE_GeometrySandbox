// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BaseGeometryyActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, Color, const FString&, Name);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinished, AActor*);

UENUM(BlueprintType)
enum class EMovementType  : uint8
{
	Sin,
	Static
};

USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Amplitude = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Frequency = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementType MoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category = "Design")
	float TimerRate = 3.f;

	FString ToString()
	{
		FString MoveTypeString;
		switch (MoveType)
		{
		case EMovementType::Static:
			MoveTypeString = "EMovementType::Static";
			break;
		case EMovementType::Sin:
			MoveTypeString = "EMovementType::Sin";
			break;
		default: break;
		}
		return FString::Printf(TEXT("%f %f %s %s %f"), Amplitude, Frequency, *MoveTypeString, *Color.ToString(), TimerRate);
	}
	FString ToHumanReadableString()
	{
		FString MoveTypeString;
		switch (MoveType)
		{
		case EMovementType::Static:
			MoveTypeString = "EMovementType::Static";
			break;
		case EMovementType::Sin:
			MoveTypeString = "EMovementType::Sin";
			break;
		default: break;
		}
		return FString::Printf(TEXT(" \n  ==GeometryData==\n  Amplitude: %f\n  Frequency:%f\n  Movement type: %s\n  Color: %s\n  Timer rate: %f"), Amplitude, Frequency, *MoveTypeString, *Color.ToString(), TimerRate);
	}
};

UCLASS()
class MY_GEOMETRYSANDBOX_API ABaseGeometryyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGeometryyActor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	void SetGeometryData(const FGeometryData& Data) { GeometryData = Data; }

	UFUNCTION(BlueprintCallable)
	FGeometryData GetGeometryData() const { return GeometryData; }

	UPROPERTY(BlueprintAssignable)
	FOnColorChanged OnColorChanged;

	FOnTimerFinished OnTimerFinished;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Geometry Data")
	FGeometryData GeometryData;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 WeaponsNum = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	int32 KillsNum = 7;

	UPROPERTY(EditInstanceOnly, Category = "Health")
	float Health = 22.251212;

	UPROPERTY(EditAnywhere, Category = "Health")
	bool IsDead = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool HasWeapon = true;

	UPROPERTY(EditAnywhere)
	FString Name = "John Connor";

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector InitialLocation;
	FTimerHandle TimerHandle;

	const int32 MaxTimerCount = 5;
	int32 TimerCount = 0;

	void PrintTypes();
	void PrintStringTypes();
	void PrintTransform();
	void HandeMovement();
	void SetColor(const FLinearColor& color);

	void OnTimerFired();
};
