// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketExplosion.h"
#include <PhysicsEngine/RadialForceComponent.h>
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>

// Sets default values
ARocketExplosion::ARocketExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	radialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	SetRootComponent(radialForceComp);
	radialForceComp->Radius = 500.f;
	radialForceComp->ImpulseStrength = 40000.f;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	sphereComp->SetupAttachment(RootComponent);
	sphereComp->SetSphereRadius(500.f);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UParticleSystem>tempExplosionFX(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Weapon/MilitaryWeapSilver/FX/P_RocketLauncher_Explosion_01.P_RocketLauncher_Explosion_01'"));
	if (tempExplosionFX.Succeeded())
	{
		particleComp->SetTemplate(tempExplosionFX.Object);
	}

}	

// Called when the game starts or when spawned
void ARocketExplosion::BeginPlay()
{
	Super::BeginPlay();

	radialForceComp->FireImpulse();
	FTimerHandle destroyTimer;
	GetWorld()->GetTimerManager().SetTimer(destroyTimer, this, &ARocketExplosion::SelfDestroy, 2.f, false);
}

// Called every frame
void ARocketExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARocketExplosion::SelfDestroy()
{
	Destroy();
}

