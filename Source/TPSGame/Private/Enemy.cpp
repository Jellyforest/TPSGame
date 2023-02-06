// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

	//1.���̷�Ż�޽� �����ͷε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Zoombie/copzombie_l_actisdato_UE.copzombie_l_actisdato_UE'"));
	//1-1���̷�Ż �޽� �����ͷε� ������
	if (tempMesh.Succeeded())
	{
		//1-2. �����͸� �Ҵ����ش�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-3. �޽���ġ�� ȸ������campsuleComp
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

		

	}
	//EnemyFSM������Ʈ �߰�
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	//�ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/CoreUObject.Class'/Script/TPSGame.EnemyAnim'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	//���忡 ��ġ�ǰų� �����ɶ� �ڵ�����
	//AIController���� Possess�ɼ� �ֵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ������ �ڽ�
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage BoxComp"));
	boxComp->SetBoxExtent(FVector(40, 40, 10));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyPlayer* player = Cast<AMyPlayer>(OtherActor);
	if (player)
	{
		player->PlayerDamagedProcess(1);
	}
}
