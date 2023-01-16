// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//1.���̷�Ż�޽� �����ͷε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Zoombie/character.character'"));
	//1-1���̷�Ż �޽� �����ͷε� ������
	if (tempMesh.Succeeded())
	{
		//1-2. �����͸� �Ҵ����ش�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//1-3. �޽���ġ�� ȸ������
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//EnemyFSM������Ʈ �߰�
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	//�ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UAnimInstance>tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy'"));
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

