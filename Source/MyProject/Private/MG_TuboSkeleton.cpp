// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_TuboSkeleton.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
AMG_TuboSkeleton::AMG_TuboSkeleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMG_TuboSkeleton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SkelMeshCompones = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (SkelMeshCompones)
	{
		TArray<FConstraintInstanceAccessor> ConstAccessorArray;
		SkelMeshCompones->GetConstraints(true, ConstAccessorArray);
		
		for (FConstraintInstanceAccessor& ConstAccessor : ConstAccessorArray)
		{
			FConstraintInstance* ConstInst = ConstAccessor.Get();

			ConstInst->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, AngularSwingLimite1);
			ConstInst->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, AngularSwingLimite2);

			ConstInst->SetSoftSwingLimitParams(true, Stiffness, 1.0f, 0.0f, 1.0f);
			
			ConstAccessor.Modify();
		}
	}
}


// Called when the game starts or when spawned
void AMG_TuboSkeleton::BeginPlay()
{
	Super::BeginPlay();

	if (SkelMeshCompones)
	{
		// Here the mass is calculated. UBodySetup::CalculateMass(const UPrimitiveComponent* Component) const
		Masa = SkelMeshCompones->GetMass();
	}
}

// Called every frame
void AMG_TuboSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkelMeshCompones)
	{
		TArray<FConstraintInstanceAccessor> ConstAccessorArray;
		SkelMeshCompones->GetConstraints(true, ConstAccessorArray);

		for (FConstraintInstanceAccessor& ConstAccessor : ConstAccessorArray)
		{
			FConstraintInstance* ConstInst = ConstAccessor.Get();

			float StiffnessSuave = ConstInst->GetSoftSwingLimitStiffness();
			FString Mensaje = FString::Printf(TEXT("Stiffness Skeleton : %f, masa %f"), StiffnessSuave, SkelMeshCompones->GetMass());
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, Mensaje);
		}
	}
}

