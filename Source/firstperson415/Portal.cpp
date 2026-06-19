#include "Portal.h"
#include "firstperson415Character.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInterface.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    mesh->SetupAttachment(Root);

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    boxComp->SetupAttachment(Root);

    sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    sceneCapture->SetupAttachment(Root);

    
}

void APortal::BeginPlay()
{
    Super::BeginPlay();

    boxComp->OnComponentBeginOverlap.AddDynamic(
        this,
        &APortal::OnOverlapBegin);

    mesh->SetHiddenInSceneCapture(true);
	mesh->bCastDynamicShadow = false;
	

    if (mat)
    {
        mesh->SetMaterial(0, mat);
    }

    if (renderTarget)
    {
        sceneCapture->TextureTarget = renderTarget;
    }
}

void APortal::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    Afirstperson415Character* playerChar =
        Cast<Afirstperson415Character>(OtherActor);

    if (playerChar)
    {
        if (OtherPortal)
        {
            if (!playerChar->isTeleporting)
            {
                playerChar->isTeleporting = true;

                FVector Loc =
                    OtherPortal->GetActorLocation();

                FRotator ExitRotation =
                    OtherPortal->GetActorRotation();

                ExitRotation.Yaw += 180.f;

                playerChar->SetActorLocation(Loc);
                playerChar->SetActorRotation(ExitRotation);

                FTimerHandle TimerHandle;
                FTimerDelegate TimerDelegate;

                TimerDelegate.BindUFunction(
                    this,
                    "SetBool",
                    playerChar);

                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandle,
                    TimerDelegate,
                    1.0f,
                    false);
            }
        }
    }
}

void APortal::SetBool(
    Afirstperson415Character* playerChar)
{
    if (playerChar)
    {
        playerChar->isTeleporting = false;
    }
}

void APortal::UpdatePortals()
{
    if (!OtherPortal || !sceneCapture)
    {
        return;
    }

    FVector Location =
        GetActorLocation() -
        OtherPortal->GetActorLocation();

    FVector camLocation =
        UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)
        ->GetTransformComponent()
        ->GetComponentLocation();

    FRotator camRotation =
        UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)
        ->GetTransformComponent()
        ->GetComponentRotation();

    FVector CombinedLocation =
        camLocation + Location;

    sceneCapture->SetWorldLocationAndRotation(
        CombinedLocation,
        camRotation);
}

void APortal::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdatePortals();
}