// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_Interactuador.h"

// Sets default values
AMG_Interactuador::AMG_Interactuador()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMG_Interactuador::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG_Interactuador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG_Interactuador::MoverAdelante(float Val)
{
    if (Val != 0.f)
    {
        if (Controller)
        {
            FRotator const ControlSpaceRot = Controller->GetControlRotation();
                        
            // transform to world space and add it
            AddMovementInput(ControlSpaceRot.Vector(), Val); //FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
        }
    }
}

void AMG_Interactuador::MoverLados(float Val)
{
    if (Val != 0.f)
    {
        if (Controller)
        {
            FRotator const ControlSpaceRot = Controller->GetControlRotation();

            // transform to world space and add it
            AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
        }
    }
}

// Called to bind functionality to input
void AMG_Interactuador::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
    PlayerInputComponent->BindAxis("MoverAdelante", this, &AMG_Interactuador::MoverAdelante);
    PlayerInputComponent->BindAxis("MoverLados", this, &AMG_Interactuador::MoverLados);
    //PlayerInputComponent->BindAxis("EjercerFuerza", this, &AMG_Interactuador::EjercerFuerza);

    PlayerInputComponent->BindAxis("Girar", this, &AMG_Interactuador::AddControllerYawInput);
    PlayerInputComponent->BindAxis("Mirar", this, &AMG_Interactuador::AddControllerPitchInput);

    //PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AMG_Interactuador::PickObject);

}

