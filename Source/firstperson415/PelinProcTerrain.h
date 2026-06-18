#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PelinProcTerrain.generated.h"

class UMaterialInterface;

UCLASS()
class FIRSTPERSON415_API APelinProcTerrain : public AActor
{
    GENERATED_BODY()

public:

    APelinProcTerrain();

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int32 XSize = 0;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    int32 YSize = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
    float ZMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
    float NoiseScale = 0.f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float Scale = 100.f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
    float UVScale = 1.f;

    UPROPERTY(EditAnywhere)
    float radius = 100.f;

    UPROPERTY(EditAnywhere)
    FVector Depth = FVector(0.f, 0.f, 10.f);

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* Mat;

public:

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void AlterMesh(FVector impactPoint);

private:

    UPROPERTY()
    UProceduralMeshComponent* ProcMesh;

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector2D> UVs;
    TArray<FVector> Normals;
    TArray<FLinearColor> UpVertexColors;
    TArray<FProcMeshTangent> Tangents;

    int32 sectionID = 0;

    void CreateVertices();
    void CreateTriangles();
};