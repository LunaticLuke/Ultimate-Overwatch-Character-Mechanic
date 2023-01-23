// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "CPP_Roadhog_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CPP_Roadhog_Secondary_Projectile.h"

// Sets default values
ACPP_Roadhog_Secondary_Projectile::ACPP_Roadhog_Secondary_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	coll = CreateDefaultSubobject<USphereComponent>("ProjectileCollider");
	SetRootComponent(coll);

	projMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	projMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	projMesh->AttachTo(coll);


	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	projectileMovement->InitialSpeed = 5700.0f;
	projectileMovement->MaxSpeed = 5700.0f;
	
}

// Called when the game starts or when spawned
void ACPP_Roadhog_Secondary_Projectile::BeginPlay()
{
	Super::BeginPlay();
	coll->OnComponentHit.AddDynamic(this, &ACPP_Roadhog_Secondary_Projectile::OnHit);

	initialLocation = GetActorLocation();
}

// Called every frame
void ACPP_Roadhog_Secondary_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector distanceTravelledVec = GetActorLocation() - initialLocation;
	float distaceTravelled = distanceTravelledVec.Size();
	if (distaceTravelled >= 900 && !hasReachedLocation)
	{
		hasReachedLocation = true;

		for (int i = 0; i < 25; i++)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FRotator spawnRotation = GetActorRotation();
			FVector spawnPosition = GetActorLocation();

			float weaponSpread = 5;

		
			FRotator randRot = spawnRotation;
			randRot.Roll += UKismetMathLibrary::RandomFloatInRange(-weaponSpread, weaponSpread);
			randRot.Pitch += UKismetMathLibrary::RandomFloatInRange(-weaponSpread, weaponSpread);
			randRot.Yaw += UKismetMathLibrary::RandomFloatInRange(-weaponSpread, weaponSpread);


			GetWorld()->SpawnActor<ACPP_Roadhog_Projectile>(roadhogProjectileClass, spawnPosition,randRot, ActorSpawnParams);
		}
		Destroy();
	}
}

void ACPP_Roadhog_Secondary_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 7, GetActorLocation());
		Destroy();
	}

	if (OtherComp->ComponentHasTag("Wall"))
	{
		Destroy();
	}
}

void ACPP_Roadhog_Secondary_Projectile::setSpreadProjectile(TSubclassOf<class ACPP_Roadhog_Projectile> proj)
{
	roadhogProjectileClass = proj;
}

