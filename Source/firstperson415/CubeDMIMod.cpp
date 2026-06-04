#include "CubeDMIMod.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
    PrimaryActorTick.bCanEverTick = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));

    RootComponent = boxComp;
    cubeMesh->SetupAttachment(boxComp);
}

void ACubeDMIMod::BeginPlay()
{
    Super::BeginPlay();

    boxComp->OnComponentBeginOverlap.AddDynamic(
        this,
        &ACubeDMIMod::OnOverlapBegin
    );

    if (baseMat)
    {
        dmiMat = UMaterialInstanceDynamic::Create(baseMat, this);
    }

    if (cubeMesh && dmiMat)
    {
        cubeMesh->SetMaterial(0, dmiMat);
    }
}

void ACubeDMIMod::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACubeDMIMod::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
        float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
        float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

        FLinearColor randColor(
            ranNumX,
            ranNumY,
            ranNumZ,
            1.f
        );

        if (dmiMat)
        {
            dmiMat->SetVectorParameterValue(TEXT("Color"), randColor);
            dmiMat->SetScalarParameterValue(TEXT("Darkness"), ranNumX);
        }
    }
}