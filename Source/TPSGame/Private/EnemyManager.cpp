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
	createTime = FMath::RandRange(minTime, maxTime);

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

	if (gm->bCanSpawnZombie && stage == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTime);
		stage++;
	}

	if (!gm->bCanSpawnZombie && stage == 1)
	{
		GetWorldTimerManager().ClearTimer(spawnTimerHandle);
		stage++;
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
		float createTimer = FMath::RandRange(minTime, maxTime);
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreateEnemy, createTimer);
	}
}

