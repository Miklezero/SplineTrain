
#pragma once

#include "RailroadTrack.h"
#include "RailroadSwitcher.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bogie.generated.h"

//∆елезнодорожна€ тележка осуществл€ет движение по сплайну, который €вл€етс€ компонентом рельсового пути или стрелочного ветвлени€
//The railway bogie provides movement along a spline, which is a component of a rail track or turnout

UCLASS()
class SPLINETRAIN_API ABogie : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABogie();

protected:
	
	virtual void BeginPlay() override;

	

public:	
	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* bogieMesh;
	UPROPERTY(EditAnywhere)
		AActor* startTrack;
		
	UPROPERTY(EditAnywhere)
		float distance = 0.0;
	UPROPERTY(EditAnywhere)
		float speed = 0.0;
	UPROPERTY(EditAnywhere)
		//1 for 0 start distance at spline or -1 for spline length distance
		int direct = 1;

private:
	USplineComponent* spline = nullptr;
	ABogie* leading = nullptr;
	ABogie* slave = nullptr;

	float distanceToLeading = 0.0;

	USceneComponent* defaultSceneComponent = nullptr;

	UBoxComponent* trigger = nullptr;

	

	
	
	
	

	FVector position = FVector::ZeroVector;

	FRotator rotation = FRotator::ZeroRotator;

public:

	void SetDistanceToLeading(float _distance);
	UFUNCTION(BlueprintCallable)
	void SetDirect(int _direct);

	UFUNCTION(BlueprintCallable)
		int GetDirect();

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float _speed);
	

	void SetMeshForBogie(UStaticMesh* _bogieMesh);
	void SetSplineForBogie(USplineComponent* _spline);

	void SetDistance(float deltaTime);
	void SetTransformForThis();

	USplineComponent* GetSpline();
	float GetDistance();

	

	FVector GetPosition();
	FRotator GetRotation();

	UFUNCTION()
		void BeginOverlayTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
