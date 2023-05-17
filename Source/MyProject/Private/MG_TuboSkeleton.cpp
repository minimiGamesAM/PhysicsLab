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
	
	Pusher = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Pusher"));
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

		HandlesForConstraint.SetNum(NbBones);
		
		for (int i = 0; i < SkelMeshCompones->GetNumBones(); ++i)
		{
			UPhysicsHandleComponent* PhyHandle = NewObject<UPhysicsHandleComponent>(this);
			PhyHandle->RegisterComponent();
			HandlesForConstraint[i] = PhyHandle;
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

	if (SplineGuide)
	{
		for (int i = 0; i < SkelMeshCompones->GetNumBones(); ++i)
		{
			UPrimitiveComponent* PriComp = Cast<UPrimitiveComponent>(SkelMeshCompones);
			FName BoneName = SkelMeshCompones->GetBoneName(i);
			FVector SocketPos = SkelMeshCompones->GetSocketLocation(BoneName);
			
			HandlesForConstraint[i]->GrabComponentAtLocation(PriComp, BoneName, SocketPos);
			HandlesForConstraint[i]->SetLinearStiffness(LinearStiffnessInSpline);

			if (i == 0)
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
			for (int i = BoneStart; i < SkelMeshCompones->GetNumBones(); ++i)
			{
				FName BoneName = SkelMeshCompones->GetBoneName(i);
				FVector SocketPos = SkelMeshCompones->GetSocketLocation(BoneName);

				FVector InertiaTensor = SkelMeshCompones->GetInertiaTensor(BoneName);

				// ConstInst->GetSoftSwingLimitStiffness();
				FString Mensaje = FString::Printf(TEXT("%s: %f, %f, %f"), *BoneName.ToString(), InertiaTensor[0], InertiaTensor[1], InertiaTensor[2]);
				GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, Mensaje);

				if (SplineGuide->SplineComp)
				{
					FVector Pos = SplineGuide->SplineComp->FindLocationClosestToWorldLocation(SocketPos, ESplineCoordinateSpace::World);
					HandlesForConstraint[i]->SetTargetLocation(Pos);

					if (i == BoneStart)
					{
						FVector PosAtBeginSpline = SplineGuide->SplineComp->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
						Pusher->SetTargetLocation(PosAtBeginSpline);
					}
				}
			}
		}
	}
}

void AMG_TuboSkeleton::InsertTubo(float DeltaInsertion)
{
	if (SplineGuide && SkelMeshCompones)
	{
		USplineComponent* SplineComp = SplineGuide->SplineComp;
		if (SplineComp)
		{
			FVector PosInSpline = SplineComp->GetLocationAtDistanceAlongSpline(DeltaInsertion, ESplineCoordinateSpace::World);

			FVector BoneLoc;
			FName BoneName = SkelMeshCompones->FindClosestBone(PosInSpline, &BoneLoc);

			BoneStart = SkelMeshCompones->GetBoneIndex(BoneName);

			UPrimitiveComponent* PriComp = Cast<UPrimitiveComponent>(SkelMeshCompones);
			Pusher->GrabComponentAtLocation(PriComp, BoneName, BoneLoc);

			for (int i = 0; i < BoneStart; ++i)
			{
				HandlesForConstraint[i]->ReleaseComponent();
			}
		}
	}
}

