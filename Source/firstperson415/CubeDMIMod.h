#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "CubeDMIMod.generated.h"

UCLASS()
class FIRSTPERSON415_API ACubeDMIMod : public AActor
{
    GENERATED_BODY()

public:
    ACubeDMIMod();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UBoxComponent* boxComp;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* cubeMesh;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* baseMat;

    UPROPERTY()
    UMaterialInstanceDynamic* dmiMat;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};