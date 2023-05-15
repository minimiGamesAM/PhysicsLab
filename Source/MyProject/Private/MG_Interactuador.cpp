// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_Interactuador.h"

#include "MG_DragComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMG_Interactuador::AMG_Interactuador()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DragComp = CreateDefaultSubobject<UMG_DragComponent>("DragComp");
    PhyHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>("PhyHandleComp");
}

// Called when the game starts or when spawned
void AMG_Interactuador::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMG_Interactuador::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    CameraComp = FindComponentByClass<UCameraComponent>();
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

void AMG_Interactuador::PickObject()
{
    if (DragComp->BeginDrag())
    {
        if (CameraComp)
        {
            CameraComp->bUsePawnControlRotation = false;
        }
    }
}

void AMG_Interactuador::ReleaseObject()
{
    DragComp->EndDrag();
    CameraComp->bUsePawnControlRotation = true;

    UE_LOG(LogTemp, Warning, TEXT("SOLTANDO OBJETO."));
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

    PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AMG_Interactuador::PickObject);
    PlayerInputComponent->BindAction("Pick", IE_Released, this, &AMG_Interactuador::ReleaseObject);

}

