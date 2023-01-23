// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_Roadhog_Projectile.h"

// Sets default values
ACPP_Roadhog_Projectile::ACPP_Roadhog_Projectile()
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
void ACPP_Roadhog_Projectile::BeginPlay()
{
	Super::BeginPlay();
	coll->OnComponentHit.AddDynamic(this, &ACPP_Roadhog_Projectile::OnHit);
	
}

void ACPP_Roadhog_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 7, GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hit"));
		Destroy();
	}
	if (OtherComp->ComponentHasTag("Wall"))
	{
		Destroy();
	}
}

// Called every frame
void ACPP_Roadhog_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

