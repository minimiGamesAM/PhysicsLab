// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_Spline.h"

#include "Components/SplineComponent.h"

// Sets default values
AMG_Spline::AMG_Spline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMG_Spline::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SplineComp = FindComponentByClass<USplineComponent>();
}

// Called when the game starts or when spawned
void AMG_Spline::BeginPlay()
{
	Super::BeginPlay();
	SplineComp = FindComponentByClass<USplineComponent>();
}

// Called every frame
void AMG_Spline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

