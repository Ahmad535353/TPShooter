// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
// #include "Engine/World.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent> (TEXT("RootComp"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent> (TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	// UE_LOG(LogTemp, Display, TEXT("Trigger!"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));	

	APawn* OwnerPawn = Cast<APawn> (GetOwner());
	if (OwnerPawn == nullptr)
	{
		return;
	}
	AController* OwnerPawnController = OwnerPawn->GetController();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	OwnerPawnController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	FVector EndOfLineTrace = PlayerViewPointLocation + PlayerViewPointRotation.Vector() *MaxRangeOfBullet; 
	
	FHitResult BulletHitResult;
	// DrawDebugLine(GetWorld(), PlayerViewPointLocation, EndOfLineTrace, FColor::Red, true);
	bool Hit = GetWorld()->LineTraceSingleByChannel(BulletHitResult, PlayerViewPointLocation, EndOfLineTrace, ECollisionChannel::ECC_GameTraceChannel1);
	if (Hit)
	{
		// DrawDebugCamera(GetWorld(), PlayerViewPointLocation, PlayerViewPointRotation, 90, 2, FColor::Red, true);
		DrawDebugPoint(GetWorld(), BulletHitResult.Location, 10, FColor::Red, true);
		FVector ShotDirection = -PlayerViewPointRotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFlash ,BulletHitResult.Location, ShotDirection.Rotation());
		AActor* DamagedActor = BulletHitResult.GetActor();
		if (DamagedActor)
		{
			FPointDamageEvent DamageEvent(DamageAmount, BulletHitResult, ShotDirection, nullptr);
			DamagedActor->TakeDamage(DamageAmount, DamageEvent, OwnerPawnController, this);
		}
	}


}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

