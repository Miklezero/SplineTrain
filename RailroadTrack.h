
#pragma once

#include "General.h"


#include "Bogie.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RailroadTrack.generated.h"

UCLASS()
class SPLINETRAIN_API ARailroadTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARailroadTrack();
	virtual void OnConstruction(const FTransform& Transform) override;
private:
	bool isToLeft = true;
	USceneComponent* defaultSceneComponent = nullptr;
	UStaticMeshComponent* triggerComponent = nullptr;
	USplineComponent* relsSpline = nullptr;
	UBoxComponent* triggerBoxStart = nullptr;
	UBoxComponent* triggerBoxEnd = nullptr;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	//
	UPROPERTY()
		TArray<USplineMeshComponent*> componentsRelsSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float sect = 80;

	UFUNCTION(BlueprintCallable)
		USplineComponent* GetSpline();

	void GenerateComponents(USplineComponent* spline, TArray<USplineMeshComponent*>* components);

	UFUNCTION()
		void BeginOverlayTriggerBoxStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void BeginOverlayTriggerBoxEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
