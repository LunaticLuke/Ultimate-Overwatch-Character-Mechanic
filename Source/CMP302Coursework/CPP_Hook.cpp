// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Hook.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerSphere.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACPP_Hook::ACPP_Hook()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	hookMesh = CreateDefaultSubobject<UStaticMeshComponent>("HookMesh");
	hookMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(hookMesh);


	coll = CreateDefaultSubobject<USphereComponent>("HookCollider");
	coll->AttachTo(hookMesh);


	hookMovement = CreateDefaultSubobject<UProjectileMovementComponent>("HookMovement");
	hookMovement->InitialSpeed = 0.0f;
	hookMovement->MaxSpeed = 0.0f;

	hookChain = CreateDefaultSubobject<UCableComponent>("Chain");
	hookChain->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	hookChain->AttachTo(hookMesh);

	

}


// Called when the game starts or when spawned
void ACPP_Hook::BeginPlay()
{
	Super::BeginPlay();
	FTransform transformInfo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform();
	Origin = transformInfo;
	coll->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Hook::BeginOverlap);
	coll->OnComponentEndOverlap.AddDynamic(this, &ACPP_Hook::OnOverlapEnd);
}


void ACPP_Hook::setOrigin(USceneComponent* comp,USceneComponent* hookComp, AActor* emptyGO)
{
	m_Hook_Origin = comp;
	hookedItemDest = hookComp;
	hook_Origin_GO = emptyGO;
}

void ACPP_Hook::startHook(float range)
{
	startPoint = m_Hook_Origin->GetComponentLocation();
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector camForward = camManager->GetCameraRotation().Vector();

	endPoint = (camForward * range) + startPoint;
	//endPoint = (camForward * range);
	DrawDebugPoint(GetWorld(), endPoint, 100, FColor::Green, true);
	if (HookSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HookSound, GetActorLocation());
	}
	firing = true;
	delayTime = 0.0f;
}

bool ACPP_Hook::getHookingStatus()
{
	return firing;
}


void ACPP_Hook::BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("Enemy") || OtherComp->ComponentHasTag("Environment") && !hookedComp && firing)
	{
		hookedComp = OtherComp;
		hooked = true;
		hookedComp->AttachToComponent(coll,FAttachmentTransformRules::KeepWorldTransform);
		itemStartPoint = hookedComp->GetComponentLocation();
		if (HookImpactSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HookImpactSound, GetActorLocation());
		}
	}
}

void ACPP_Hook::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedComp->ComponentHasTag("Enemy"))
	{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stopped Colliding"));
	}
}
	

// Called every frame
void ACPP_Hook::Tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, m_Hook_Origin->GetComponentLocation().ToString());
	Super::Tick(DeltaTime);
	if (!firing)
	{
		FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		Origin.TransformPosition(FVector(playerPos.X - 4, playerPos.Y, playerPos.Z + 5));
		SetActorLocation(m_Hook_Origin->GetComponentLocation());
		SetActorRotation(m_Hook_Origin->GetComponentQuat());
	}
	else
	{
		//Rebounding = on the way back to origin
		if (rebounding)
		{
			lerpedPos = FMath::Lerp(endPoint, m_Hook_Origin->GetComponentLocation(), lerpValue * (hookSpeed/2));
			SetActorLocation(lerpedPos, false, NULL, ETeleportType::TeleportPhysics);
			if (hookedComp)
			{
				float dist = FVector::Dist(m_Hook_Origin->GetComponentLocation(), GetActorLocation());
				float distItem = FVector::Dist(hookedItemDest->GetComponentLocation(), hookedComp->GetComponentLocation());
				
				if (distItem <= 4.0f || dist <= 2.0f || lerpValue >= 1.0f)
				{
					
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Item Within Range"));
					hookedComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
					hookedComp->SetWorldLocation(hookedItemDest->GetComponentLocation(), false, NULL, ETeleportType::TeleportPhysics);
					hookedComp->BodyInstance.bLockRotation = false;
					hookedComp = NULL;
					firing = false;
					rebounding = false;
					hooked = false;
					lerpValue = 0;
				}
				else
				{
				lerpedPos = FMath::Lerp(itemStartPoint, hookedItemDest->GetComponentLocation(), lerpValue * (hookSpeed/2));
				hookedComp->SetWorldLocation(lerpedPos,false,NULL,ETeleportType::TeleportPhysics);
				
				}
			}
			else
			{

			float dist = FVector::Dist(m_Hook_Origin->GetComponentLocation(), GetActorLocation());
			if (dist <= 2 || lerpValue >= 1.0f)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Hook Within Range"));
				firing = false;
				rebounding = false;
				if (hookedComp)
				{
					hookedComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
					hookedComp->SetWorldLocation(hookedItemDest->GetComponentLocation(), false, NULL, ETeleportType::TeleportPhysics);
					hookedComp->BodyInstance.bLockRotation = false;
					hookedComp = NULL;
					hooked = false;
				}
				lerpValue = 0;
			}

			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(dist));
			lerpValue += 0.75 *  DeltaTime;
		}
		else
		{
			lerpedPos = FMath::Lerp(startPoint, endPoint, lerpValue * (hookSpeed * 2));
			SetActorLocation(lerpedPos);
			if (!hookedComp)
			{
				lerpValue += 2 * DeltaTime;
			}

	

			if (lerpValue >= 1 || hookedComp)
			{
				if (hookedComp)
				{
					if (delayTime >= 0.25f)
					{
						rebounding = true;
						lerpValue = 0;
						endPoint = GetActorLocation();
					}
					else
					{
						delayTime += DeltaTime;
					}
				}
				else
				{
					rebounding = true;
					lerpValue = 0;
					endPoint = GetActorLocation();
				}
			}

		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Rebounding") + rebounding);

	//DrawDebugSphere(GetWorld(),m_Hook_Origin->,  1000, 26, FColor(181, 0, 0), true, -1, 0, 2);

}

