// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_TuboSkeleton.generated.h"

class USkeletalMeshComponent;
class UPhysicsHandleComponent;
class AMG_Spline;

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

	UPROPERTY(VisibleAnywhere, Category = "Connection")
	int32 NbBones = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Body")
	USkeletalMeshComponent* SkelMeshCompones;

	UPROPERTY(EditAnywhere, Category = "Constraint")
	AMG_Spline* SplineGuide = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Constraint")
	TArray<UPhysicsHandleComponent*> HandlesForConstraint;

	UPROPERTY(EditAnywhere, Category = "Constraint")
	float LinearStiffnessInSpline = 2000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Constraint")
	UPhysicsHandleComponent* Pusher;

	UPROPERTY(EditAnywhere, Category = "Navigacion")
	int32 BoneStart = 0;

	//UPROPERTY(EditAnywhere, Category = "Constraint")
	//UPhysicsHandleComponent* MyPhysicsHandle = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Navigacion")
	void InsertTubo(float DeltaInsertion);

};
