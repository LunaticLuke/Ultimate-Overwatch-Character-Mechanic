// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "CPP_Hook.generated.h"

UCLASS(ClassGroup = (Overwatch), meta = (BlueprintSpawnableComponent), Blueprintable)
class CMP302COURSEWORK_API ACPP_Hook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Hook();
	UPROPERTY(EditAnywhere,Category = "Components")
	class UStaticMeshComponent* hookMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UProjectileMovementComponent* hookMovement;
	UPROPERTY(EditAnywhere, Category = " Components");
	class USphereComponent *coll;
	UPrimitiveComponent* hookedComp;
	UPROPERTY()
	FTransform Origin;
	UPROPERTY()
	class USceneComponent*  m_Hook_Origin;
	UPROPERTY()
	class USceneComponent* hookedItemDest;
	UPROPERTY(EditAnywhere)
	float hookSpeed = 5;
	UPROPERTY()
	FVector endPoint;
	FVector startPoint;
	FVector lerpedPos;
	FVector itemStartPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* hook_Origin_GO;

	float delayTime = 0.0f;
	//Current lerp progress
	float lerpValue = 0;
	//Has something been hooked?
	bool hooked = false;
	//Is the hook on its way back to the player?
	bool rebounding = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCableComponent* hookChain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* HookSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* HookImpactSound;
	UPROPERTY()
	bool firing = false;
	void setOrigin(class USceneComponent* comp,class USceneComponent* hookComp, AActor* emptyGO);
	
	void startHook(float range);
	bool getHookingStatus();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
