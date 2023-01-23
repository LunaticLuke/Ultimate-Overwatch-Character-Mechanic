// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Roadhog_Projectile.generated.h"

UCLASS(ClassGroup = (Overwatch), meta = (BlueprintSpawnableComponent), Blueprintable)
class CMP302COURSEWORK_API ACPP_Roadhog_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Roadhog_Projectile();
	UPROPERTY(EditAnywhere, Category = "Components")
		class UStaticMeshComponent* projMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UProjectileMovementComponent* projectileMovement;
	UPROPERTY(EditAnywhere, Category = "Components");
		class USphereComponent* coll;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
