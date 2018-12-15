// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BallProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Hearing.h"
#include "Components/SphereComponent.h"

// TODO find out why InitSphereRadius can't be changed
/*	EXPLANATION - it CAN be changed and it does get changed here but it is the radius of the Collision Component and not the mesh itself.
	StaticMesh can also be changed here if need be. */
// TODO add sound effects on collision
// TODO add a second weapon - just a stretched hand or something simple enough

ABallProjectile::ABallProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABallProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 3500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
	// Adding a Particle effect for death of projectile
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("/Game/PersonalAssets/Particles/P_Explosion"));
	ProjectileImpactEffect = Particle.Object;

	// Adds a sound to the projectile ( OnHit )
	static ConstructorHelpers::FObjectFinder<USoundBase> Soundf(TEXT("/Game/PersonalAssets/Audio/ImpactBlast"));
	ProjectileImpactSound = Soundf.Object;
}

void ABallProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		// Activating the Particle effect and emitting sound of impact 
		if (ProjectileImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileImpactEffect, GetActorLocation());
			if (ProjectileImpactSound)
			{
				// TODO Edit SoundAtLocation to have a falloff with distance
				UGameplayStatics::PlaySoundAtLocation(this, ProjectileImpactSound, GetActorLocation());
				UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation());
			}
		}
		Destroy();
	}
}