
#include "RailroadSwitcher.h"


ARailroadSwitcher::ARailroadSwitcher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponentn"));
	SetRootComponent(defaultSceneComponent);
	defaultSceneComponent->SetMobility(EComponentMobility::Static);

#pragma region Splines

	leftSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LeftSpline"));
	leftSpline->SetupAttachment(RootComponent);
	leftSpline->SetMobility(EComponentMobility::Static);
	//to left
	//3-point
	leftSpline->AddSplineLocalPoint(FVector(150.0, -1.0, 0.0));
	//leftSpline->SetTangentAtSplinePoint(2, (FVector(20.0, 0.0, 0.0)), ESplineCoordinateSpace::Local, true);
	//4-point
	leftSpline->AddSplineLocalPoint(FVector(300.0, -6.0, 0.0));
	//leftSpline->SetTangentAtSplinePoint(3, (FVector(50.0, -2.5, 0.0)), ESplineCoordinateSpace::Local, true);leftRelsSpline
	//4-point
	leftSpline->AddSplineLocalPoint(FVector(500.0, -15.0, 0.0));
	//leftSpline->SetTangentAtSplinePoint(3, (FVector(50.0, -5.0, 0.0)), ESplineCoordinateSpace::Local, true);
	//5-point
	leftSpline->AddSplineLocalPoint(FVector(700.0, -30.0, 0.0));
	//6-point
	leftSpline->AddSplineLocalPoint(FVector(1200.0, -80.0, 0.0));
	//7-point
	leftSpline->AddSplineLocalPoint(FVector(1600.0, -121.0, 0.0));

	rightSpline = CreateDefaultSubobject<USplineComponent>(TEXT("RightSpline"));
	rightSpline->SetupAttachment(RootComponent);
	rightSpline->SetMobility(EComponentMobility::Static);

	rightSpline->SetLocationAtSplinePoint(2,
		FVector(rightSpline->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::Local).X, 0.0, -0.01), ESplineCoordinateSpace::Local);
	//to right
	//3-point
	rightSpline->AddSplineLocalPoint(FVector(150.0, 2.0, -0.01));
	//rightSpline->SetTangentAtSplinePoint(2, (FVector(20.0, 0.0, -0.01)), ESplineCoordinateSpace::Local, true);
	//4-point
	rightSpline->AddSplineLocalPoint(FVector(300.0, 6.0, -0.01));
	//rightSpline->SetTangentAtSplinePoint(3, (FVector(50.0, 2.5, -0.01)), ESplineCoordinateSpace::Local, true);
	//4-point
	rightSpline->AddSplineLocalPoint(FVector(500.0, 15.0, -0.01));
	//rightSpline->SetTangentAtSplinePoint(3, (FVector(50.0, 5.0, -0.01)), ESplineCoordinateSpace::Local, true);
	//5-point
	rightSpline->AddSplineLocalPoint(FVector(700.0, 30.0, -0.01));
	//6-point
	rightSpline->AddSplineLocalPoint(FVector(1200.0, 80.0, -0.01));
	//7-point
	rightSpline->AddSplineLocalPoint(FVector(1600.0, 121.0, -0.01));

#pragma endregion

	triggerBoxStart = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentStart"));
	triggerBoxStart->SetupAttachment(RootComponent);
	triggerBoxStart->SetMobility(EComponentMobility::Static);
	triggerBoxStart->AddLocalOffset(FVector(64.0, 0.0, 54));
	triggerBoxStart->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FVector posEndPoint = GetLocationEndSplinePoint(leftSpline);
	triggerBoxLeftEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentLeftEnd"));
	triggerBoxLeftEnd->SetupAttachment(RootComponent);
	triggerBoxLeftEnd->SetMobility(EComponentMobility::Static);
	triggerBoxLeftEnd->AddLocalOffset(FVector(posEndPoint.X - 64, posEndPoint.Y, posEndPoint.Z + 54));
	triggerBoxLeftEnd->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	 posEndPoint = GetLocationEndSplinePoint(rightSpline);
	triggerBoxRightEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentRightEnd"));
	triggerBoxRightEnd->SetupAttachment(RootComponent);
	triggerBoxRightEnd->SetMobility(EComponentMobility::Static);
	triggerBoxRightEnd->AddLocalOffset(FVector(posEndPoint.X - 64, posEndPoint.Y, posEndPoint.Z + 54));
	triggerBoxRightEnd->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARailroadSwitcher::BeginPlay()
{
	Super::BeginPlay();

}

void ARailroadSwitcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARailroadSwitcher::OnConstruction(const FTransform& Transform)
{
	GenerateComponents(leftSpline, &componentsLeftSpline);
	GenerateComponents(rightSpline, &componentsRightSpline);
	UE_LOG(LogTemp, Warning, TEXT("Constructor: %s"), "Message");
}

void ARailroadSwitcher::GenerateComponents(USplineComponent* spline, TArray<USplineMeshComponent*>* components)
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
}

FVector ARailroadSwitcher::GetLocationEndSplinePoint(USplineComponent* spline)
{
	int number = spline->GetNumberOfSplinePoints();
	return spline->GetLocationAtSplinePoint(number == 0 ? 0 : number - 1, ESplineCoordinateSpace::Local);
}

void ARailroadSwitcher::ToLeft()
{
	isToLeft = true;
}

void ARailroadSwitcher::ToRight()
{
	isToLeft = false;
}

USplineComponent* ARailroadSwitcher::GetSpline()
{
	if (isToLeft) return leftSpline;
	return rightSpline;
}

