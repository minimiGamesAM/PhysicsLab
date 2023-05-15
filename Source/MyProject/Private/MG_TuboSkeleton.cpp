// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_TuboSkeleton.h"

#include "MG_Spline.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AMG_TuboSkeleton::AMG_TuboSkeleton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 32; ++i)
	{
		FName HandleName = FName(FString::Printf(TEXT("PhyHandleNombre%i"), i));
		UPhysicsHandleComponent* PhComp = CreateDefaultSubobject<UPhysicsHandleComponent>(HandleName);
	}

	Pusher = CreateDefaultSubobject<UPhysicsHandleComponent>("Pusher");
}

void AMG_TuboSkeleton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//FName HandleName = FName(FString::Printf(TEXT("PhyHandleNombre%i"), 0));
	//MyPhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>("PhCmp");
	//
	//HandlesForConstraint.Add(MyPhysicsHandle);


	SkelMeshCompones = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (SkelMeshCompones)
	{
		TArray<FConstraintInstanceAccessor> ConstAccessorArray;
		SkelMeshCompones->GetConstraints(true, ConstAccessorArray);
		
		for (FConstraintInstanceAccessor& ConstAccessor : ConstAccessorArray)
		{
			FConstraintInstance* ConstInst = ConstAccessor.Get();

			EAngularConstraintMotion MotionType = EAngularConstraintMotion::ACM_Free;

			
			if (MovTipo == 1)
			{
				MotionType = EAngularConstraintMotion::ACM_Limited;
			}
			else if(MovTipo == 2)
			{
				MotionType = EAngularConstraintMotion::ACM_Locked;
			}
						
			ConstInst->SetAngularSwing1Limit(MotionType, AngularSwingLimite1);
			ConstInst->SetAngularSwing2Limit(MotionType, AngularSwingLimite2);

			ConstInst->SetSoftSwingLimitParams(true, Stiffness, 1.0f, 0.0f, 1.0f);
			
			ConstAccessor.Modify();
		}

		NbBones = SkelMeshCompones->GetNumBones();
				
		//for (int i = 0; i < NbBones; ++i)
		//{
		//	FName HandleName = FName(FString::Printf(TEXT("PhyHandleNombre%i"), i));
		//	UPhysicsHandleComponent* PhComp = CreateDefaultSubobject<UPhysicsHandleComponent>(HandleName);
		//
		//	HandlesForConstraint.Add(PhComp);
		//}
	}
}

// Called when the game starts or when spawned
void AMG_TuboSkeleton::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> comps;

	this->GetComponents(comps);

	HandlesForConstraint.Empty();

	for (int i = 0; i < comps.Num(); ++i) //Because there may be more components
	{
		UPhysicsHandleComponent* thisCompP = Cast<UPhysicsHandleComponent>(comps[i]); //try to cast to static mesh component
		if (thisCompP)
		{
			HandlesForConstraint.Add(thisCompP);
		}
	}

	if (SkelMeshCompones)
	{
		// Here the mass is calculated. UBodySetup::CalculateMass(const UPrimitiveComponent* Component) const
		Masa = SkelMeshCompones->GetMass();
	}

	if (SplineGuide)
	{
		for (int i = 0; i < HandlesForConstraint.Num(); ++i)
		{
			UPrimitiveComponent* PriComp = Cast<UPrimitiveComponent>(SkelMeshCompones);
			FName BoneName = SkelMeshCompones->GetBoneName(i);
			FVector SocketPos = SkelMeshCompones->GetSocketLocation(BoneName);

			HandlesForConstraint[i]->GrabComponentAtLocation(PriComp, BoneName, SocketPos);

			if (i == HandlesForConstraint.Num() - 1)
			{
				Pusher->GrabComponentAtLocation(PriComp, BoneName, SocketPos);
			}
		}
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

		//for (FConstraintInstanceAccessor& ConstAccessor : ConstAccessorArray)
		//{
			//FConstraintInstance* ConstInst = ConstAccessor.Get();

			//float StiffnessSuave = ConstInst->GetSoftSwingLimitStiffness();
			//FString Mensaje = FString::Printf(TEXT("Stiffness Skeleton : %f, masa %f"), StiffnessSuave, SkelMeshCompones->GetMass());
			//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, Mensaje);
		//}

		if (SplineGuide)
		{
			for (int i = 0; i < HandlesForConstraint.Num(); ++i)
			{
				FName BoneName = SkelMeshCompones->GetBoneName(i);
				FVector SocketPos = SkelMeshCompones->GetSocketLocation(BoneName);

				//SplineGuide = Cast<AMG_Spline>(SplineGuide.GetDefaultObject()); // Cast<AMG_Spline>(SplineGuide.Get());
				if (SplineGuide->SplineComp)
				{
					FVector Pos = SplineGuide->SplineComp->FindLocationClosestToWorldLocation(SocketPos, ESplineCoordinateSpace::World);
					HandlesForConstraint[i]->SetTargetLocation(Pos);

					if (i == HandlesForConstraint.Num() - 1)
					{
						FVector PosSplineEnd = SplineGuide->SplineComp->GetLocationAtDistanceAlongSpline(SplineGuide->SplineComp->GetSplineLength(), ESplineCoordinateSpace::World);
						Pusher->SetTargetLocation(PosSplineEnd);
					}
				}
			}
		}
	}
}

