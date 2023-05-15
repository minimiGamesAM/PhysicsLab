// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MG_Interactuador.generated.h"

class UMG_DragComponent;
class UPhysicsHandleComponent;
class UCameraComponent;

UCLASS()
class MYPROJECT_API AMG_Interactuador : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMG_Interactuador();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	UMG_DragComponent* DragComp;

	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* PhyHandleComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void MoverAdelante(float Val);

	void MoverLados(float Val);

	void PickObject();

	void ReleaseObject();

};
