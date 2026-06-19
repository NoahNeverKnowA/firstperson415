#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"


class APortal;
class Afirstperson415Character;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UStaticMeshComponent;
class UBoxComponent;
class UMaterialInterface;

UCLASS()
class FIRSTPERSON415_API APortal : public AActor
{
    GENERATED_BODY()

public:

    APortal();

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneCaptureComponent2D* sceneCapture;

    UPROPERTY(EditAnywhere)
    UArrowComponent* rootArrow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTextureRenderTarget2D* renderTarget;

    UPROPERTY(EditAnywhere)
    UBoxComponent* boxComp;

    UPROPERTY(EditAnywhere)
    APortal* OtherPortal;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* mat;

	UPROPERTY(EditAnywhere)
    USceneComponent* Root;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void SetBool(
        Afirstperson415Character* playerChar);

    void UpdatePortals();

protected:

    virtual void BeginPlay() override;
};