// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "RocketAmmo.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include <Particles/ParticleSystem.h>
#include <Blueprint/UserWidget.h>
#include <Components/CapsuleComponent.h>
#include <Components/StaticMeshComponent.h>
#include "EnemyFSM.h"
#include "Grenade.h"
#include "MyPlayerAnim.h"
#include "RocketAmmoPre.h"
#include "Animation/AnimSequence.h"
#include "Engine/StaticMeshSocket.h"
#include <Components/SplineComponent.h>
#include "WorldWarGameMode.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ĳ���� �޽�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Player/Ch17_nonPBR_UE.Ch17_nonPBR_UE'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));
	// ��������
	playerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("playerSpringArm"));
	playerSpringArm->SetupAttachment(RootComponent);
	playerSpringArm->TargetArmLength = 250;
	// ī�޶�
	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("playerCamera"));
	playerCamera->SetupAttachment(playerSpringArm);
	playerCamera->SetRelativeLocation(FVector(0, 50, 100));

	bUseControllerRotationYaw = true;
	playerSpringArm->bUsePawnControlRotation = true;
	playerCamera->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ���Ϸ�ó
	rocketLauncherComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rocket Launcher Mesh"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempRocketLauncher(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Weapon/MilitaryWeapSilver/Weapons/Rocket_Launcher_A.Rocket_Launcher_A'"));
	if (tempRocketLauncher.Succeeded())
	{
		rocketLauncherComp->SetSkeletalMesh(tempRocketLauncher.Object);
	}
	rocketLauncherComp->SetupAttachment(GetMesh(), TEXT("handLSoc"));
	rocketLauncherComp->SetRelativeLocationAndRotation(FVector(25.32f, 1.74f, -11.17f), FRotator(19.04f, -243.28f, -13.75f));
	// ������
	rifleComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle Mesh"));
	rifleComp->SetupAttachment(GetMesh(), TEXT("handLSoc"));
	rifleComp->SetRelativeLocationAndRotation(FVector(25.32f, 1.74f, -11.17f), FRotator(19.04f, -243.28f, -13.75f));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempRifle(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Weapon/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (tempRifle.Succeeded())
	{
		rifleComp->SetSkeletalMesh(tempRifle.Object);
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem>tempBulletEffect(TEXT("/Script/Engine.ParticleSystem'/Game/Assets/Weapon/MilitaryWeapSilver/FX/P_Impact_Metal_Medium_01.P_Impact_Metal_Medium_01'"));
	if (tempBulletEffect.Succeeded())
	{
		bulletEffectFactory = tempBulletEffect.Object;
	}
	// ������
	knifeComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Knife Mesh"));
	knifeComp->SetupAttachment(GetMesh(), TEXT("handRSoc"));
	knifeComp->SetRelativeLocationAndRotation(FVector(97.76f, -26.10f, 5.42f), FRotator(89.13f, -192.53f, 177.53f));
	knifeComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempKnife(TEXT("/Script/Engine.StaticMesh'/Game/Construction_VOL2/Meshes/SM_Shovel_01.SM_Shovel_01'"));
    if (tempKnife.Succeeded())
    {
	    knifeComp->SetStaticMesh(tempKnife.Object);
    }
	// ����ź
	grenadeComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Greande Mesh"));
	grenadeComp->SetupAttachment(GetMesh(), TEXT("handRSoc"));
	grenadeComp->SetRelativeLocation(FVector(-30, 0, 120));
	grenadeComp->SetRelativeLocationAndRotation(FVector(0.65f, -9.24f, 4.f), FRotator(0.f, -15.f, 0.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempGrenade(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Weapon/MilitaryWeapSilver/Weapons/GrenadeLauncherA_Ammo.GrenadeLauncherA_Ammo'"));
	if (tempGrenade.Succeeded())
    {
	    grenadeComp->SetStaticMesh(tempGrenade.Object);
    }
	grenadeComp->SetRelativeScale3D(FVector(2.f));
	// ����ź ���� ���ö���
	splineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	splineComp->SetupAttachment(GetMesh());
	splineComp->SetRelativeLocation(FVector(8.f, 27.f, 120.f));

}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	// ĳ����
	anim = Cast<UMyPlayerAnim>(GetMesh()->GetAnimInstance());
	gm = Cast<AWorldWarGameMode>(GetWorld()->GetAuthGameMode());
	// ����
	crossZoomUI = CreateWidget(GetWorld(), crossZoomFactory);
	crossHitUI = CreateWidget(GetWorld(), crossHitFactory);
	crossIdleUI = CreateWidget(GetWorld(), crossIdleFactory);
	warningTextUI = CreateWidget(GetWorld(), warningTextFactory);
	crossIdleUI->AddToViewport();

	// ���۽� ���
	ArmRifle();

	knifeComp->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::KnifeOverlap);

	// �⺻ �ȱ� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ������� dir�� �÷��̾���� dir�� ��ȭ�ϱ� ���� �÷��̾� rotaion�� Ʈ���������� ����
	FTransform playerTrans(GetControlRotation());
	// �÷��̾� Ʈ�������� �°� dir ��ȯ
	FVector newDir = playerTrans.TransformVector(dir);
	// Z�� ���� (�ٴ��������ϰ�)
	newDir.Z = 0;
	newDir.Normalize();
	// ��ȯ�� dir�� �̵� (�ӵ��� self��)
	AddMovementInput(newDir);

	// ����ź ���� ��ġ
	grenadeFireLoc = GetActorLocation() + FVector(8.f, 27.f, 120.f);

	// ����ź ���� velocity�� �÷��̾� �����̼ǿ� �°� �ְ� �ʹ�
	grenadeLaunchVelocity = playerTrans.TransformVector(FVector(1.f, 0, 0.5f));

	if (bIsGrenadeAiming)
	{
		PredictGrenadePath();
	}
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayer::InputAxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &AMyPlayer::InputAxisTurnRight);
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AMyPlayer::InputAxisMoveVertical);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AMyPlayer::InputAxisMoveHorizontal);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyPlayer::InputActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPlayer::InputActionFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyPlayer::InputActionFireReleased);
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &AMyPlayer::ArmRifle);
	PlayerInputComponent->BindAction(TEXT("RocketLauncher"), IE_Pressed, this, &AMyPlayer::ArmRocketLauncher);
	PlayerInputComponent->BindAction(TEXT("Knife"), IE_Pressed, this, &AMyPlayer::ArmKnife);
	PlayerInputComponent->BindAction(TEXT("Grenade"), IE_Pressed, this, &AMyPlayer::PlaySetGrenadeAnim);
	PlayerInputComponent->BindAction(TEXT("Grenade"), IE_Released, this, &AMyPlayer::PlayThrowGrenadeAnim);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &AMyPlayer::Zoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &AMyPlayer::ZoomOut);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMyPlayer::InputActionRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMyPlayer::InputActionRunReleased);
}

void AMyPlayer::InputAxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void AMyPlayer::InputAxisTurnRight(float value)
{
	AddControllerYawInput(value);
}

void AMyPlayer::InputAxisMoveVertical(float value)
{
	dir.X = value;
}

void AMyPlayer::InputAxisMoveHorizontal(float value)
{
	dir.Y = value;
}

void AMyPlayer::InputActionJump()
{
	Jump();
}

void AMyPlayer::InputActionFire()
{
	switch (nowWeapon)
	{
	case WeaponList::Rifle:
		GetWorldTimerManager().SetTimer(rifleTimerhandle, this, &AMyPlayer::FireRifle, fireRifleInterval, true);
		FireRifle();
		break;
	case WeaponList::RocketLauncher:
		FireRocketLauncher();
		break;
	case WeaponList::Knife:
		bIsKnifeAttackPressing = true;
		FireKnife();
		break;
	default:
		break;
	}
}

void AMyPlayer::InputActionFireReleased()
{
	switch (nowWeapon)
	{
	case WeaponList::Rifle:
		GetWorldTimerManager().ClearTimer(rifleTimerhandle);
		break;
	case WeaponList::RocketLauncher:
		break;
	case WeaponList::Knife:
		bIsKnifeAttackPressing = false;
		break;
	default:
		break;
	}
}

void AMyPlayer::InputActionRun()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void AMyPlayer::InputActionRunReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void AMyPlayer::ArmRifle()
{
	ChangeWeapon(WeaponList::Rifle);
}

void AMyPlayer::ArmRocketLauncher()
{
	ChangeWeapon(WeaponList::RocketLauncher);
}

void AMyPlayer::ArmKnife()
{
	ChangeWeapon(WeaponList::Knife);
}

void AMyPlayer::ChangeWeapon(WeaponList value)
{
	switch (value)
	{
	case
		WeaponList::Rifle:
			nowWeapon = value;
			rifleComp->SetVisibility(true);
			rocketLauncherComp->SetVisibility(false);
			knifeComp->SetVisibility(false);
			grenadeComp->SetVisibility(false);
			anim->bIsKnifeMode = false;
		break;
	case 
		WeaponList::RocketLauncher:
			nowWeapon = value;
			rifleComp->SetVisibility(false);
			rocketLauncherComp->SetVisibility(true);
			knifeComp->SetVisibility(false);
			grenadeComp->SetVisibility(false);
			anim->bIsKnifeMode = false;
		break;
	case
		WeaponList::Knife:
			ChangeWeaponZooming();
			nowWeapon = value;
			rifleComp->SetVisibility(false);
			rocketLauncherComp->SetVisibility(false);
			knifeComp->SetVisibility(true);
			grenadeComp->SetVisibility(false);
			anim->bIsKnifeMode = true;
		break;
	default:
		break;
	}
}

void AMyPlayer::FireRifle()
{
	// ����Ʈ���̽�
	FHitResult hitInfo;
	FVector startLoc = playerCamera->GetComponentLocation();
	FVector endLoc = startLoc + playerCamera->GetForwardVector() * 100000.f;
	FCollisionQueryParams param;
	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, param);
	if (isHit)
	{
		FTransform trans(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, trans);
		// �� ������
		AEnemy* enemy = Cast<AEnemy>(hitInfo.GetActor());
		if (enemy)
		{
			UEnemyFSM* enemyfsm = Cast<UEnemyFSM>(enemy->fsm);
			if (enemyfsm)
			{
				enemyfsm->OnDamageProcess(1);
				CrossHit();
 			}
		}
	}
	// �ݵ�
	AddControllerPitchInput(-0.15f);
	AddControllerYawInput(FMath::RandRange(-0.1f, 0.1f));

	// ������ �ִϸ��̼�
	rifleComp->PlayAnimation(animRifleFire, false);

	// �ݵ� �ִϸ��̼�
	if (anim)
	{
		anim->FireAnim();
	}
}

void AMyPlayer::FireRocketLauncher()
{
	FVector rocketLoc = rocketLauncherComp->GetSocketLocation(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ARocketAmmoPre>(rocketFactory, rocketLoc, GetControlRotation());

	rocketLauncherComp->PlayAnimation(animRockerLauncherFire, false);

	// ī�޶� ����ũ
	auto playerCamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	playerCamManager->StartCameraShake(rocketCamShakeFactory);
}

void AMyPlayer::FireKnife()
{
	if (anim)
	{
		anim->PlayKnifeAttackAnim(TEXT("FirstAttack"));
	}
}

void AMyPlayer::FireGrenade()
{
	AActor* greActor = GetWorld()->SpawnActor<AGrenade>(grenadeFactory, grenadeFireLoc, GetControlRotation());
}

void AMyPlayer::Zoom()
{
	switch (nowWeapon)
	{
	case WeaponList::Rifle:
		crossIdleUI->RemoveFromParent();
		crossZoomUI->AddToViewport();
		playerCamera->SetFieldOfView(60.f);
		bIsZooming = true;
		break;
	case WeaponList::RocketLauncher:
		crossIdleUI->RemoveFromParent();
		crossZoomUI->AddToViewport();
		playerCamera->SetFieldOfView(60.f);
		bIsZooming = true;
		break;
	case WeaponList::Knife:
		break;
	default:
		break;
	}
}

void AMyPlayer::ZoomOut()
{
	switch (nowWeapon)
	{
	case WeaponList::Rifle:
		crossZoomUI->RemoveFromParent();
		crossIdleUI->AddToViewport();
		playerCamera->SetFieldOfView(90.f);
		bIsZooming = false;
		break;
	case WeaponList::RocketLauncher:
		crossZoomUI->RemoveFromParent();
		crossIdleUI->AddToViewport();
		playerCamera->SetFieldOfView(90.f);
		bIsZooming = false;
		break;
	case WeaponList::Knife:
		break;
	default:
		break;
	}
}

void AMyPlayer::CrossHit()
{
	if (!bIsHitUIOn)
	{
		bIsHitUIOn = true;
		crossHitUI->AddToViewport();
		GetWorld()->GetTimerManager().SetTimer(crossHitTimerhandle, FTimerDelegate::CreateLambda([&]()
			{
				crossHitUI->RemoveFromParent();
				bIsHitUIOn = false;
			}), 1.f, false);
	}
}

void AMyPlayer::CountdownTimer(int time)
{
	gm->currentCountdown = time;
	GetWorldTimerManager().SetTimer(countdownHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (gm->currentCountdown > 0)
			{
				gm->currentCountdown--;
			}
			else
			{
				gm->currentCountdown = 0;
				gm->bCanSpawnZombie = !gm->bCanSpawnZombie;
				GetWorldTimerManager().ClearTimer(countdownHandle);
				//UE_LOG(LogTemp, Warning, TEXT("timeout"));
			}
		}),1.f , true);
}

void AMyPlayer::PlayerDamagedProcess(int value)
{
	if (playerHP > 0)
	{
		playerHP -= value;
		UE_LOG(LogTemp, Warning, TEXT("outch"));
	}
	else
	{
		anim->bIsDead = true;
		playerCamera->SetRelativeLocationAndRotation(FVector(200.f, 0.f, 600.f), FRotator(270.f, 0.f, 0.f));
		bUseControllerRotationYaw = false;
		playerSpringArm->bUsePawnControlRotation = false;
		playerCamera->bUsePawnControlRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMyPlayer::ChangeWeaponZooming()
{
	if (nowWeapon == WeaponList::Rifle || nowWeapon == WeaponList::RocketLauncher && bIsZooming)
	{
		crossZoomUI->RemoveFromParent();
		crossIdleUI->AddToViewport();
		playerCamera->SetFieldOfView(90);
		bIsZooming = false;
	}
}

void AMyPlayer::PlaySetGrenadeAnim()
{
	if (anim && !anim->bIsKnifeMode)
	{
		anim->PlayGrenadeAnim(TEXT("Set"));
	}
}

void AMyPlayer::PlayThrowGrenadeAnim()
{
	if (anim && !anim->bIsKnifeMode)
	{
		anim->PlayGrenadeAnim(TEXT("Go"));
	}
}

void AMyPlayer::PredictGrenadePath()
{
	FPredictProjectilePathResult predictResult;
	FPredictProjectilePathParams predictParam;
	predictParam.StartLocation = grenadeFireLoc;
	predictParam.LaunchVelocity = grenadeLaunchVelocity * 1000.f;
	predictParam.DrawDebugType = EDrawDebugTrace::None;
	predictParam.SimFrequency = 20.f;
	predictParam.MaxSimTime = 10.f;
	UGameplayStatics::PredictProjectilePath(GetWorld(), predictParam, predictResult);

	for (FPredictProjectilePathPointData pathDataToLoc : predictResult.PathData)
	{
		predictPathLoc.Add(pathDataToLoc.Location);
	}
	DrawGrenadePath();
	predictPathLoc.Empty();
}

void AMyPlayer::KnifeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	if (enemy)
	{
		UEnemyFSM* fsm = Cast<UEnemyFSM>(enemy->fsm);
		if (fsm)
		{
			fsm->OnDamageProcess(1);
			CrossHit();
		}
	}
}

