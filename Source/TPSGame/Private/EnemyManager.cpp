// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include "WorldWarGameMode.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	//���������ð� ���ϱ�
	createTime = FMath::RandRange(1, 3);

	//2.Timer Manager���� �˶� ���
	//GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
	gm = Cast<AWorldWarGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
/*
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > waveTime)
	{
		CreateEnemy();
	}
	currentTime = 0;
*/

	if (gm->bCanSpawnZombie && gm->stage == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
		gm->stage++;
	}

	if (!gm->bCanSpawnZombie && gm->stage == 2)
	{
		GetWorldTimerManager().ClearTimer(spawnTimerHandle);
		gm->stage++;
	}

	if (!bDoOnce && gm->stage == 3)
	{
		bDoOnce = true;
		gm->ShowEnding();
	}
}


void AEnemyManager::CreateEnemy()
{
	{
		//���� ��ġ���ϱ�
		int index = FMath::RandRange(0, spawnPoints.Num() - 1);
		//�� ���� �׹�ġ�ϱ�
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));

		//�ٽ� ���� �ð��� CreateEnemy�Լ��� ȣ��ǵ��� ���̸� ����
		float createTimer = FMath::RandRange(1, 3);
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTimer);
	}
}

