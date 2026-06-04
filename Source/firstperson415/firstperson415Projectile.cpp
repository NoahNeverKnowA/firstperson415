// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstperson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

Afirstperson415Projectile::Afirstperson415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &Afirstperson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;

	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void Afirstperson415Projectile::BeginPlay()
{
    Super::BeginPlay();
    randColor = FLinearColor(
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        UKismetMathLibrary::RandomFloatInRange(0.f, 1.f),
        1.f
    );

    dmiMat = UMaterialInstanceDynamic::Create(projMat, this);

    ballMesh->SetMaterial(0, dmiMat);

    dmiMat->SetVectorParameterValue("ProjColor", randColor);
}

void Afirstperson415Projectile::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != nullptr) &&
        (OtherActor != this) &&
        (OtherComp != nullptr) &&
        OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(
            GetVelocity() * 100.0f,
            GetActorLocation());

        Destroy();
    }

    if (OtherActor != nullptr)
    {
        float frameNum =
            UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

        if (baseMat == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("BASEMAT IS NULL"));
            return;
        }

        auto Decal =
            UGameplayStatics::SpawnDecalAtLocation(
                GetWorld(),
                baseMat,
                FVector(
                    UKismetMathLibrary::RandomFloatInRange(
                        20.f,
                        40.f)),
                Hit.Location,
                Hit.Normal.Rotation(),
                0.f);

        if (Decal == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("DECAL IS NULL"));
            return;
        }

        auto MatInstance =
            Decal->CreateDynamicMaterialInstance();

        if (MatInstance == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("MAT INSTANCE IS NULL"));
            return;
        }

        MatInstance->SetVectorParameterValue(
            TEXT("Color"),
            randColor);

        MatInstance->SetScalarParameterValue(
            TEXT("Frame"),
            frameNum);
    }
}