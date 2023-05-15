// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Math/Quat.h"

#include "MG_DragComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGrabObject, AActor*, InstigatorActor, UPrimitiveComponent*, CompToDrag, FVector, Location);

class UPhysicsHandleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMG_DragComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PhysicsComp")
	static UPhysicsHandleComponent* GetPhysicsHandleComp(AActor* FromActor);

public:	
	// Sets default values for this component's properties
	UMG_DragComponent();

	bool BeginDrag();

	void EndDrag();

	UPROPERTY(BlueprintAssignable)
	FOnGrabObject OnGrabObject;

	UPROPERTY(VisibleAnywhere)
	FVector LocalPos;

	UPROPERTY(VisibleAnywhere)
	FQuat4d LocalRot;

	UPROPERTY(VisibleAnywhere)
	bool PossesObject;

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
