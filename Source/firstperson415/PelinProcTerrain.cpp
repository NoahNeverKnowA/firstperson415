#include "PelinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInterface.h"

APelinProcTerrain::APelinProcTerrain()
{
    PrimaryActorTick.bCanEverTick = false;

    ProcMesh =
        CreateDefaultSubobject<UProceduralMeshComponent>(
            TEXT("Procedural Mesh"));

    RootComponent = ProcMesh;
}

void APelinProcTerrain::BeginPlay()
{
    Super::BeginPlay();

    CreateVertices();
    CreateTriangles();

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
        Vertices,
        Triangles,
        UVs,
        Normals,
        Tangents);

    ProcMesh->CreateMeshSection_LinearColor(
        sectionID,
        Vertices,
        Triangles,
        Normals,
        UVs,
        UpVertexColors,
        Tangents,
        true);

    if (Mat)
    {
        ProcMesh->SetMaterial(0, Mat);
    }
}

void APelinProcTerrain::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APelinProcTerrain::AlterMesh(FVector impactPoint)
{
    FVector LocalImpactPoint =
        impactPoint - GetActorLocation();

    for (int32 i = 0; i < Vertices.Num(); i++)
    {
        if ((Vertices[i] - LocalImpactPoint).Size() < radius)
        {
            Vertices[i] -= Depth;
        }
    }

    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
        Vertices,
        Triangles,
        UVs,
        Normals,
        Tangents);

    ProcMesh->UpdateMeshSection_LinearColor(
        sectionID,
        Vertices,
        Normals,
        UVs,
        UpVertexColors,
        Tangents);
}

void APelinProcTerrain::CreateVertices()
{
    Vertices.Empty();
    UVs.Empty();

    for (int32 X = 0; X <= XSize; X++)
    {
        for (int32 Y = 0; Y <= YSize; Y++)
        {
            float Z =
                FMath::PerlinNoise2D(
                    FVector2D(
                        X * NoiseScale + 0.1f,
                        Y * NoiseScale + 0.1f))
                * ZMultiplier;

            Vertices.Add(
                FVector(
                    X * Scale,
                    Y * Scale,
                    Z));

            UVs.Add(
                FVector2D(
                    X * UVScale,
                    Y * UVScale));
        }
    }
}

void APelinProcTerrain::CreateTriangles()
{
    Triangles.Empty();

    int32 Vertex = 0;

    for (int32 X = 0; X < XSize; X++)
    {
        for (int32 Y = 0; Y < YSize; Y++)
        {
            Triangles.Add(Vertex);
            Triangles.Add(Vertex + 1);
            Triangles.Add(Vertex + YSize + 1);

            Triangles.Add(Vertex + 1);
            Triangles.Add(Vertex + YSize + 2);
            Triangles.Add(Vertex + YSize + 1);

            Vertex++;
        }

        Vertex++;
    }
}