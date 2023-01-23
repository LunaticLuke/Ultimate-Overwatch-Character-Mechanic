// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Roadhog_Secondary_Projectile.generated.h"

UCLASS()
class CMP302COURSEWORK_API ACPP_Roadhog_Secondary_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Roadhog_Secondary_Projectile();
	UPROPERTY(EditAnywhere, Category = "Components")
		class UStaticMeshComponent* projMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UProjectileMovementComponent* projectileMovement;
	UPROPERTY(EditAnywhere, Category = "Components");
	class USphereComponent* coll;
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class ACPP_Roadhog_Projectile> roadhogProjectileClass;
	FVector initialLocation;
	bool hasReachedLocation = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void setSpreadProjectile(TSubclassOf<class ACPP_Roadhog_Projectile> proj);

};
