// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//���忡�� �÷��̾� Ÿ�� ��������

	//ĳ����

	//������ü��������

}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}

}
//������ - (���Ŀ� �÷��̾ ��Ÿ��������) 
void UEnemyFSM::IdleState()
{
	// �ð��� �帣�ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	//���� ����ð��� ���ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		//3. �̵����·� ��ȯ�ϰ�ʹ�
		mState = EEnemyState::Move;
		//4. ����ð� �ʱ�ȭ
		currentTime = 0;
		UE_LOG(LogTemp, Warning, TEXT("2after move"));
	}
}
//�̵�����
void UEnemyFSM::MoveState()
{
	//Ÿ�� ������

	// ����

	//�������� �̵��ϰ�ʹ�

}
//���ݻ���
void UEnemyFSM::AttackState(){}
//�ǰݻ���
void UEnemyFSM::DamageState(){}
//��������
void UEnemyFSM::DieState(){}

