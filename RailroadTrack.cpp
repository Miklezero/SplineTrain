// Fill out your copyright notice in the Description page of Project Settings.


#include "RailroadTrack.h"

ARailroadTrack::ARailroadTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponentn"));
	SetRootComponent(defaultSceneComponent);
	defaultSceneComponent->SetMobility(EComponentMobility::Static);

	relsSpline = CreateDefaultSubobject<USplineComponent>(TEXT("relsSpline"));
	relsSpline->SetupAttachment(RootComponent);
	relsSpline->SetMobility(EComponentMobility::Static);

	triggerBoxStart = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentStart"));
	triggerBoxStart->SetupAttachment(RootComponent);
	triggerBoxStart->SetMobility(EComponentMobility::Static);
	triggerBoxStart->AddLocalOffset(FVector(64.0, 0.0, 54));
	triggerBoxStart->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	FVector posSecondPoint = relsSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	triggerBoxEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentEnd"));
	triggerBoxEnd->SetupAttachment(RootComponent);
	triggerBoxEnd->SetMobility(EComponentMobility::Static);
	triggerBoxEnd->AddLocalOffset(FVector(posSecondPoint.X - 64, posSecondPoint.Y, posSecondPoint.Z + 54));
	triggerBoxEnd->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ARailroadTrack::BeginPlay()
{
	Super::BeginPlay();
}

void ARailroadTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARailroadTrack::OnConstruction(const FTransform& Transform)
{
	GenerateComponents(relsSpline, &componentsRelsSpline);

}

USplineComponent* ARailroadTrack::GetSpline()
{
	return relsSpline;
}

void ARailroadTrack::GenerateComponents(USplineComponent* spline, TArray<USplineMeshComponent*>* components)
{
	if (!spline)
	{
		return;
	}

	if (components->Num() > 0)
	{
		for (int32 i = 0; i < components->Num(); i++)
		{
			if ((*components)[i])
			{
				(*components)[i]->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				(*components)[i]->DestroyComponent();
			}
		}
		components->Empty();
	}
	if (sect == 0) sect = spline->GetSplineLength();
	int countSegms = (int)(spline->GetSplineLength() / sect);

	for (size_t i = 0; i < countSegms; i++)
	{
		USplineMeshComponent* addingSplineMesh = NewObject<USplineMeshComponent>(this);
		addingSplineMesh->RegisterComponent();
		addingSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		//UE_LOG(LogTemp, Display, TEXT("Added new USplineMeshComponent: %s"), *addingSplineMesh->GetName());


		float distanceStart = i * sect;
		float distanceEnd = (i + 1) * sect;
		//UE_LOG(LogTemp, Display, TEXT("Start/End distance: %d %f / %f"), i, distanceStart, distanceEnd);

		FVector startPos = spline->GetLocationAtDistanceAlongSpline(sect * i, ESplineCoordinateSpace::Local);
		FVector startTang = spline->GetTangentAtDistanceAlongSpline(sect * i, ESplineCoordinateSpace::Local);
		startTang = startTang.GetClampedToSize(0, sect);

		FVector endPos = spline->GetLocationAtDistanceAlongSpline(sect * (i + 1), ESplineCoordinateSpace::Local);
		FVector endTang = spline->GetTangentAtDistanceAlongSpline(sect * (i + 1), ESplineCoordinateSpace::Local);
		endTang = endTang.GetClampedToSize(0, sect);
		//UE_LOG(LogTemp, Display, TEXT("Start/End location and tangent: %d %s / %s"), i, *startPos.ToString(), *endPos.ToString());

		addingSplineMesh->SetForwardAxis(ESplineMeshAxis::X, true);
		addingSplineMesh->SetStartAndEnd(startPos, startTang, endPos, endTang, true);
		addingSplineMesh->SetStaticMesh(mesh);
		addingSplineMesh->AttachToComponent(spline, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		components->Add(addingSplineMesh);
	}
	//Trigger box  added to pos end point
	int32 numberOfPoints = relsSpline->GetNumberOfSplinePoints();
	FVector posEndPoint = relsSpline->GetLocationAtSplinePoint(numberOfPoints - 1, ESplineCoordinateSpace::World);
	triggerBoxEnd->SetWorldLocation(FVector(posEndPoint.X - 64, posEndPoint.Y, posEndPoint.Z + 54));
	triggerBoxEnd->SetWorldRotation(relsSpline->GetRotationAtSplinePoint(numberOfPoints - 1, ESplineCoordinateSpace::World));

}

