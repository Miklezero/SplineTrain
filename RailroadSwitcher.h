// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "General.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RailroadSwitcher.generated.h"

UCLASS()
class SPLINETRAIN_API ARailroadSwitcher : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARailroadSwitcher();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:

	bool isToLeft = true;
	USceneComponent* defaultSceneComponent = nullptr;

	USplineComponent* leftSpline = nullptr;
	USplineComponent* rightSpline = nullptr;

	UBoxComponent* triggerBoxStart = nullptr;
	UBoxComponent* triggerBoxLeftEnd = nullptr;
	UBoxComponent* triggerBoxRightEnd = nullptr;

public:

	UPROPERTY()
		TArray<USplineMeshComponent*> componentsLeftSpline;
	UPROPERTY()
		TArray<USplineMeshComponent*> componentsRightSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float sect = 80;

	FVector GetLocationEndSplinePoint(USplineComponent* spline);


	UFUNCTION(BlueprintCallable)
		void ToLeft();

	UFUNCTION(BlueprintCallable)
		void ToRight();

	UFUNCTION(BlueprintCallable)
		USplineComponent* GetSpline();

	void GenerateComponents(USplineComponent* spline, TArray<USplineMeshComponent*>* components);
};
