// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_TuboSkeleton.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MYPROJECT_API AMG_TuboSkeleton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_TuboSkeleton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere, Category = "Connection")
	int MovTipo = 1;

	UPROPERTY(VisibleAnywhere, Category = "Bodies")
	float Masa = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Connection")
	float Stiffness = 123.0f;

	UPROPERTY(EditAnywhere, Category = "Connection")
	float AngularSwingLimite1 = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Connection")
	float AngularSwingLimite2 = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Body")
	USkeletalMeshComponent* SkelMeshCompones;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
