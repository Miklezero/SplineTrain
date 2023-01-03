


#include "Bogie.h"


ABogie::ABogie()
{
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponentn"));
	SetRootComponent(defaultSceneComponent);
	defaultSceneComponent->SetMobility(EComponentMobility::Movable);

	bogieMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bogie"));
	bogieMesh->SetMobility(EComponentMobility::Movable);
	bogieMesh->SetupAttachment(RootComponent);
	bogieMesh->AddLocalOffset(FVector(0.0, 0.0, 33.0));
	bogieMesh->SetGenerateOverlapEvents(false);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("bogieMesh"));
	trigger->SetupAttachment(RootComponent);
	trigger->SetMobility(EComponentMobility::Movable);
	trigger->AddLocalOffset(FVector(0.0, 0.0, 54.0));
	trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//trigger->OnComponentBeginOverlap.AddDynamic(this, &ABogie::BeginOverlayTrigger);


}


void ABogie::BeginPlay()
{
	Super::BeginPlay();	

	ARailroadTrack* startTrack = Cast<ARailroadTrack>(actorTrack);
	if (startTrack)
	{
		spline = startTrack->GetSpline();
	}

}


void ABogie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetDistance(DeltaTime);
	SetTransformForThis();
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, FString::Printf(TEXT("Bogie: %s"), *actorTrack->GetName()));
}

void ABogie::SetDistanceToLeading(float _distance)
{
	distanceToLeading = _distance;
}

void ABogie::SetDirect(int _direct)
{
	if (_direct >= 0) direct = 1;
	else direct = -1;
}

int ABogie::GetDirect()
{
	return direct;
}

void ABogie::SetSpeed(float _speed)
{
	speed = _speed;
}

void ABogie::SetMeshForBogie(UStaticMesh* _bogieMesh)
{
	bogieMesh->SetStaticMesh(_bogieMesh);
}

void ABogie::SetSplineForBogie(USplineComponent* _spline)
{
	spline = _spline;
}

void ABogie::SetDistance(float deltaTime)
{
	if (leading && leading->GetSpline() == spline)
	{
		distance = leading->GetDistance() + direct * distanceToLeading;
	}
	else
	{
		distance = distance + direct * deltaTime * speed * correct;
	}
}

void ABogie::SetStartDistance(float startDistance)
{
	distance = startDistance;
}

void ABogie::SetTransformForThis()
{
	if (spline)
	{
		position = spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
		rotation = spline->GetRotationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);

		this->SetActorLocation(position);
		this->SetActorRotation(rotation);
	}
}

USplineComponent* ABogie::GetSpline()
{
	return spline;
}

void ABogie::SetSpline(USplineComponent* splineComponent)
{
	spline = splineComponent;
}

float ABogie::GetDistance()
{
	return distance;
}

FVector ABogie::GetPosition()
{
	return position;
}

FRotator ABogie::GetRotation()
{
	return rotation;
}

void ABogie::BeginOverlayTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	/*	ARailroadTrack* railroadObject = Cast<ARailroadTrack>(OtherActor);
		ARailroadSwitcher* switcherObject = Cast<ARailroadSwitcher>(OtherActor);

		if (railroadObject)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Beg...Over... %s"), *OtherActor->GetName()));
			USplineComponent* getSpline = railroadObject->GetSpline();
			if (spline != getSpline && speed != 0)
			{
				spline = railroadObject->GetSpline();
				
				float relativeSize = OtherComp->GetRelativeLocation().X;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("relat %f"), relativeSize));
				if (relativeSize <= 100)
				{
					distance = 0;
					direct = 1;
				}
				else
				{
					distance = spline->GetSplineLength();
					direct = -1;
				}
			}
		}
		else if (switcherObject)
		{
			if (spline != switcherObject->GetSpline())
			{
				spline = switcherObject->GetSpline();
				float relativeSize = OtherComp->GetRelativeLocation().X;
				if (relativeSize == 64) distance = 0;
				else distance = spline->GetSplineLength();
			}
		}
		else
		{
			spline = nullptr;
		}*/	
}
