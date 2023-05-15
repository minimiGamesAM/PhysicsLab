// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_Spline.generated.h"

class USplineComponent;

UCLASS()
class MYPROJECT_API AMG_Spline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_Spline();

	UPROPERTY(VisibleAnywhere, Category = "SplineComp")
	USplineComponent* SplineComp;

	UPROPERTY(EditAnywhere)
	float RadiusDebugSphere = 5.0f;

	bool PossesObject;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
