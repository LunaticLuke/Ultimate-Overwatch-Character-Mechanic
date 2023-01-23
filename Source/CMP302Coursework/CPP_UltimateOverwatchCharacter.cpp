// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_UltimateOverwatchCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UCPP_UltimateOverwatchCharacter::UCPP_UltimateOverwatchCharacter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Hook_Origin = CreateDefaultSubobject<USceneComponent>("HookOrigin");

	m_Hooked_Item_Dest = CreateDefaultSubobject<USceneComponent>("HookedItemDestination");

	FAbility ability1;
	ability1.m_ability_name = TEXT("Tracer's Recall");
	ability1.m_id = 0;
	ability1.m_has_Ability = true;
	ability1.m_Cooldown_Seconds = 6;

	FAbility ability2;
	ability2.m_ability_name = TEXT("Roadhogs's Hook");
	ability2.m_id = 1;
	ability2.m_has_Ability = true;
	ability2.m_Cooldown_Seconds = 8;
	
	abilities.Push(ability1);
	abilities.Push(ability2);

	// ...
}



// Called when the game starts
void UCPP_UltimateOverwatchCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ...


	FActorSpawnParameters spawnInfo;
	
	if (m_Hook_Origin)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Origin Successfully Stated"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Origin Not Set"));
	}

	//Fill the array data up
	for (int i = 0; i < 12; i++)
	{
		positionData.Add(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
		rotationData.Add(GetWorld()->GetFirstPlayerController()->GetControlRotation());
	}
	FTransform spawnTransform = m_Hook_Origin->GetComponentTransform();
	FActorSpawnParameters spawnParams;

	hookClass =  GetWorld()->SpawnActor<ACPP_Hook>(hookBP, spawnTransform, spawnParams);
	hookClass->setOrigin(m_Hook_Origin,m_Hooked_Item_Dest,hook_Origin_GO);
	if (hookClass)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Spawned Hook"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hook Not Set"));
	}
	
	GetWorld()->GetTimerManager().SetTimer(m_position_timer, this, &UCPP_UltimateOverwatchCharacter::StorePosition, 0.25f, true);
	GetWorld()->GetTimerManager().SetTimer(m_wallRun_Timer, this, &UCPP_UltimateOverwatchCharacter::wallRunUpdateEvent, 0.02f, true);
	defaultGravity = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement()->GravityScale;
}



// Called every frame
void UCPP_UltimateOverwatchCharacter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	displayDebugPoints();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,rotationData[0].ToString());
	if (rewinding)
	{
			float timePerPos = rewindDuration / 12.0f;
			lerpedPos = FMath::Lerp(startPos, positionData[indexOfRewind], lerpValue * lerpPosSpeed);
			lerpedRot = FMath::Lerp(startRot, rotationData[indexOfRewind], lerpValue * lerpPosSpeed);
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(lerpedPos);
			GetWorld()->GetFirstPlayerController()->SetControlRotation(lerpedRot);
			lerpValue += rewindDuration * DeltaTime;
			rotLerpValue += rewindDuration * DeltaTime;
			if (lerpValue >= timePerPos)
			{
				lerpValue = 0;
				rotLerpValue = 0;
				indexOfRewind--;
				startPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				startRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
			}
			if (indexOfRewind < 0)
			{
			rewinding = false;
			
			}
	}

	CameraTick();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,FString::SanitizeFloat());
}

UFUNCTION()
void UCPP_UltimateOverwatchCharacter::StorePosition()
{
	if (!rewinding)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Position Stored"));
		positionData.Add(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
		rotationData.Add(GetWorld()->GetFirstPlayerController()->GetControlRotation());
		if (positionData.Num() > 12)
		{
			positionData.RemoveAt(0);
		}
		if (rotationData.Num() > 12)
		{
			rotationData.RemoveAt(0);
		}
	}
}


void UCPP_UltimateOverwatchCharacter::TracerRecall()
{
	hooking = hookClass->getHookingStatus();
	if (abilities[0].m_has_Ability && !hooking)
	{
		abilities[0].m_has_Ability = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, abilities[0].m_ability_name);
		GetWorld()->GetTimerManager().SetTimer(abilities[0].m_cooldown_timer, this, &UCPP_UltimateOverwatchCharacter::endCooldownAbilityOne, 5.0f, false);

		rewinding = true;
	

		lerpValue = 0;
		startPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		startRot = GetWorld()->GetFirstPlayerController()->GetControlRotation();
		rotLerpValue = 0;
		indexOfRewind = 11;
		if (RewindSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RewindSound, startPos);
		}
	}
}

void UCPP_UltimateOverwatchCharacter::RoadhogHook()
{
	
	if (abilities[1].m_has_Ability && !rewinding)
	{
		abilities[1].m_has_Ability = false;
		hooking = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Roadhog Ability"));
		GetWorld()->GetTimerManager().SetTimer(abilities[1].m_cooldown_timer, this, &UCPP_UltimateOverwatchCharacter::endCooldownAbilityTwo, 8.0, false);
		hookClass->startHook(m_Hook_Range);
	}
}

void UCPP_UltimateOverwatchCharacter::LucioWallRun()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Lucio Ability"));
}

void UCPP_UltimateOverwatchCharacter::CassidyDeadeye()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Deadeye Ability"));
}

void UCPP_UltimateOverwatchCharacter::wallRunJump()
{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Jumping"));
	if (wallRunning)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Jumping Whilst Wall Running"));
		EndWallRun(0.35f);

		FVector wallRunJumpOffVec = FVector(jumpOffForce * wallRunNormal.X, jumpOffForce * wallRunNormal.Y, wallRunJumpHeight);

		GetWorld()->GetFirstPlayerController()->GetCharacter()->LaunchCharacter(wallRunJumpOffVec, false, true);
	}
}

void UCPP_UltimateOverwatchCharacter::CameraTick()
{
	if (wallRunningL)
	{
		CameraTilt(15.0f);
	}
	{
		if (wallRunningR)
		{
			CameraTilt(-15.0f);
		}
		else
		{
			CameraTilt(0.0f);
		}
	}
	
	

}

void UCPP_UltimateOverwatchCharacter::CameraTilt(float TargetXRoll)
{
	FRotator characterRotation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetController()->GetControlRotation();
	FRotator targetRot = FRotator(characterRotation.Pitch,characterRotation.Yaw,TargetXRoll);

	FRotator rotatedCam = FMath::RInterpTo(characterRotation, targetRot, GetWorld()->GetDeltaSeconds(), 10);

	GetWorld()->GetFirstPlayerController()->GetCharacter()->GetController()->SetControlRotation(rotatedCam);
}

void UCPP_UltimateOverwatchCharacter::EndWallRun(float resetTime)
{
	if (wallRunning)
	{
		wallRunning = false;
		wallRunningR = false;
		wallRunningL = false;
		GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement()->GravityScale = defaultGravity;
		suppressWallRun(resetTime);
	}
}

void UCPP_UltimateOverwatchCharacter::wallRunUpdateEvent()
{
	
		APawn* character = GetWorld()->GetFirstPlayerController()->GetCharacter();
		FVector characterLocation = character->GetActorLocation();
		FVector characterRight = character->GetActorRightVector();
		FVector characterForward = character->GetActorForwardVector();

		characterRight *= 75.0f; //Multiply by 75 to have a range of 75.
		characterForward *= -35.0f; // Multiply by -35 to point slightly backwards (wiggle room).

		rightRayEndPoint = characterLocation + characterRight + characterForward;

		FVector characterLeft = character->GetActorRightVector() * -75.0f;

		leftRayEndPoint = characterLocation + characterLeft + characterForward;
		DrawDebugLine(GetWorld(), characterLocation, rightRayEndPoint, FColor::Blue, false);
		DrawDebugLine(GetWorld(), characterLocation, leftRayEndPoint, FColor::Red, false);
		UCharacterMovementComponent* movement = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement();

		if (!wallRunSuppress)
		{
			if (wallRunMovement(characterLocation, rightRayEndPoint, -1.0f))
			{
				wallRunning = true;
				wallRunningR = true;
				wallRunningL = false;

				movement->GravityScale = FMath::FInterpTo(movement->GravityScale, wallRunTargetGravity, GetWorld()->DeltaTimeSeconds, 10);

			}
			else
			{
				if (!wallRunningR)
				{
					if (wallRunMovement(characterLocation, leftRayEndPoint, 1.0f))
					{
						wallRunning = true;
						wallRunningL = true;
						wallRunningR = false;

						movement->GravityScale = FMath::FInterpTo(movement->GravityScale, wallRunTargetGravity, GetWorld()->DeltaTimeSeconds, 10);
					}
					else
					{
						EndWallRun(1.0f);
					}
				}
				else
				{
					EndWallRun(1.0f);
				}
			}
		}
	
}


bool UCPP_UltimateOverwatchCharacter::wallRunMovement(FVector Start,FVector End, float Dir)
{
	FCollisionQueryParams WallRun_TraceParams = FCollisionQueryParams(FName(TEXT("WallRunTrace")), true,GetWorld()->GetFirstPlayerController()->GetPawn());

	FHitResult WallRun_Hit(ForceInit);
	UCharacterMovementComponent* movement = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement();

	GetWorld()->LineTraceSingleByChannel(WallRun_Hit, Start, End, ECC_WorldDynamic, WallRun_TraceParams);


	if (WallRun_Hit.bBlockingHit)
	{

		if (WallRun_Hit.Normal.Z > -0.52 && WallRun_Hit.Normal.Z < 0.52 && movement->IsFalling())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Normal Is In Range & character is falling"));
			wallRunNormal = WallRun_Hit.Normal;

			//Stick character to wall
			GetWorld()->GetFirstPlayerController()->GetCharacter()->LaunchCharacter(CalculatePlayerToWallVector(),false,false);

			float speedDir = wallRunSpeed * Dir;
			FVector forwardBackVector = FVector::CrossProduct(wallRunNormal, FVector(0.0f, 0.0f, 1.0));

			FVector moveVec = speedDir * forwardBackVector;
			//Move character forward
			GetWorld()->GetFirstPlayerController()->GetCharacter()->LaunchCharacter(moveVec,true,!wallRunGravity);
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}


	
}

FVector UCPP_UltimateOverwatchCharacter::CalculatePlayerToWallVector()
{
	FVector vec = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - wallRunNormal;
	
	float vecForce = vec.Size();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(vecForce));
	 //Closer to the wall you get, less force needed to stick to wall
	FVector forceVector = vecForce * wallRunNormal;

	return vec;
}

void UCPP_UltimateOverwatchCharacter::suppressWallRun(float delay)
{
	wallRunSuppress = true;
	GetWorld()->GetTimerManager().SetTimer(suppressTimer, this, &UCPP_UltimateOverwatchCharacter::resetWallRunSuppress, delay,false);
}

void UCPP_UltimateOverwatchCharacter::resetWallRunSuppress()
{
	GetWorld()->GetTimerManager().ClearTimer(suppressTimer);
	wallRunSuppress = false;
}

float UCPP_UltimateOverwatchCharacter::getCooldown(int index)
{
	if (abilities[index].m_has_Ability)
	{
		return FMath::RoundToFloat(abilities[index].m_Cooldown_Seconds);
	}
	else
	{
		float floatToDisplay = abilities[index].m_Cooldown_Seconds - GetWorld()->GetTimerManager().GetTimerElapsed(abilities[index].m_cooldown_timer);
		return FMath::RoundToFloat(floatToDisplay);
	}
}

UFUNCTION()
void UCPP_UltimateOverwatchCharacter::endCooldownAbilityOne()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stopped Rewinding"));
	abilities[0].m_has_Ability = true;
}

UFUNCTION()
void UCPP_UltimateOverwatchCharacter::endCooldownAbilityTwo()
{
	abilities[1].m_has_Ability = true;
}

void UCPP_UltimateOverwatchCharacter::endRewind()
{
	rewinding = false;
}

void UCPP_UltimateOverwatchCharacter::displayDebugPoints()
{
	FlushPersistentDebugLines(GetWorld());
	for (int i = 0; i < positionData.Num(); i++)
	{
		DrawDebugPoint(GetWorld(), positionData[i], 100, FColor::Blue, true);
	}
	DrawDebugPoint(GetWorld(), m_Hook_Origin->GetComponentLocation(), 100, FColor::Yellow, true);
}

