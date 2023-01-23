// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "CPP_Hook.h"
#include "CPP_UltimateOverwatchCharacter.generated.h"



	USTRUCT(BlueprintType)
	struct FAbility
	{
		GENERATED_BODY();

		UPROPERTY()
		int m_id = 0;
		UPROPERTY()
		FString m_ability_name = TEXT("Will Use Later");
		UPROPERTY()
		float m_Cooldown_Seconds = 3;
		UPROPERTY()
		bool m_has_Ability = false;
		UPROPERTY()
		FTimerHandle m_cooldown_timer;

	};

UCLASS( ClassGroup=(Overwatch), meta=(BlueprintSpawnableComponent), Blueprintable)
class CMP302COURSEWORK_API UCPP_UltimateOverwatchCharacter : public USceneComponent
{

	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UCPP_UltimateOverwatchCharacter();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// How far can Roadhog's hook go?
	float m_Hook_Range = 100;
	//Where does the hook emanate from?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_Hook_Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_Hooked_Item_Dest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//An array of the last 6 positions (for Tracer's Recall)
	TArray<FVector> positionData;
	TArray<FRotator> rotationData;
	FVector lerpedPos;
	FVector startPos;
	FRotator lerpedRot;
	FRotator startRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* RewindSound;
	float lerpPosSpeed = 3;
	float indexOfRewind;
	float rotLerpValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPhysicsHandleComponent* m_Character_Physics_Handle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* hook_Origin_GO;
	UPROPERTY(EditAnywhere)
	TArray<FAbility> abilities;
	bool rewinding = false;
	bool hooking = false;
	float lerpValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle m_position_timer;
	UPROPERTY(EditAnywhere)
	FTimerHandle m_rewind_timer;
	float rewindDuration = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hooking")
	TSubclassOf<class ACPP_Hook> hookBP;
	ACPP_Hook* hookClass;
	UActorComponent* hookMesh;
	UFUNCTION(BlueprintCallable)
	//This function trigger's Tracer's recall ability - sending the character back 3 seconds
	void TracerRecall();
	UFUNCTION(BlueprintCallable)
	//This function causes a hook to get sent outwards towards an enemy - reeling them in
	void RoadhogHook();
	UFUNCTION(BlueprintCallable)
	//This function allows the player to wall run in a similar manner to Lucio
	void LucioWallRun();
	UFUNCTION(BlueprintCallable)
	//This function allows the player locks onto enemies in a deadeye attack - requires full ultimate.
	void CassidyDeadeye();


	FVector leftRayEndPoint;
	FVector rightRayEndPoint;
	FVector wallRunDirection;
	bool wallRunning = false;
	bool wallRunningR = false;
	bool wallRunningL = false;
	float wallRunSpeed = 1000;
	float wallRunTargetGravity = 0.25f;
	float defaultGravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool wallRunSuppress = false;
	FTimerHandle suppressTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool wallRunGravity = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float wallRunJumpHeight = 800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float jumpOffForce = 800;
	
	UFUNCTION(BlueprintCallable)
	void wallRunJump();

	void CameraTick();
	
	void CameraTilt(float TargetXRoll);

	UFUNCTION(BlueprintCallable)
	void EndWallRun(float resetTime);

	FTimerHandle m_wallRun_Timer;


	void wallRunUpdateEvent();
	

	FVector wallRunNormal;


	bool wallRunMovement(FVector Start, FVector End, float Dir);

	FVector CalculatePlayerToWallVector();
	UFUNCTION(BlueprintCallable)
	void suppressWallRun(float delay);
	UFUNCTION(BlueprintCallable)
	void resetWallRunSuppress();

	UFUNCTION(BlueprintCallable)
	float getCooldown(int index);


	UFUNCTION(BlueprintCallable)
	void endCooldownAbilityOne();

	void endCooldownAbilityTwo();

	void endRewind();

	void displayDebugPoints();

	void StorePosition();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//An ability struct to store data about the ability.
};
