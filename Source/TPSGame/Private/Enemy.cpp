// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <Components/CapsuleComponent.h>


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

 	//campsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("campsuleComp"));
	//SetRootComponent(campsuleComp); 

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

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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

