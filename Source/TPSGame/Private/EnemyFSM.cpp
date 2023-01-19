// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
//#include "TPSPr.h"
#include "EnemyAnim.h"
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
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyPlayer::StaticClass());
	//ĳ����
	target = Cast<AMyPlayer>(actor);
	//������ü��������
	me = Cast<AEnemy>(GetOwner());

	//UEnemyAnim*�Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

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
		//UE_LOG(LogTemp, Warning, TEXT("2after move"));

		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}


}
//�̵�����
void UEnemyFSM::MoveState()
{
	//Ÿ�� ������
	FVector destination = target->GetActorLocation();
	// ����
	FVector dir = destination - me->GetActorLocation();
	//�������� �̵��ϰ�ʹ�
	me->AddMovementInput(dir.GetSafeNormal());
	
	//Ÿ��� ��������� ���ݻ��·� ��ȯ�ϰ�ʹ�
	//1. ���� �Ÿ��� ���ݹ����ȿ� ������
	if (dir.Size() < attackRange)
	{
		//2.���ݻ��·� ��ȯ�ϰ�ʹ�
		mState = EEnemyState::Attack;
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		//���ݾִϸ��̼� ���Ȱ��ȭ
		anim->bAttackPlay = true;
		//���ݻ�����ȯ�� ���ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;

	}
}
//���ݻ���
void UEnemyFSM::AttackState()
{
	

	    //��ǥ: ���� �ð��� �ѹ��� �����ϰ�ʹ�
	    //1. �ð��� �귯���Ѵ�
	    currentTime += GetWorld()->DeltaRealTimeSeconds;
		//2. ���ݽð��� ������
		if (currentTime > attackDelayTime)
		{
	
			//3. �����ϰ�ʹ�

			//---------
			currentTime = 0;
			anim->bAttackPlay = true;
	
		}

	
// 	    //��ǥ: Ÿ���� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ�ʹ�
		//1. Ÿ����� �Ÿ��� �ʿ�
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
		//2.Ÿ����� �Ÿ��� ���ݹ����� ���� ������
		if (distance > attackRange)
		{
			//3. ���¸� �̵����� ��ȯ�ϰ� �ʹ�
			mState = EEnemyState::Move;
			//�ִϸ��̼� ���� ����ȭ
			anim->animState = mState;
		}
	

}
//�ǰݻ���
void UEnemyFSM::DamageState()
{
	//1.�ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	//2.���� ����ð��� ���ð��� �ʰ��ߴٸ� 
	if (currentTime > damageDelayTime)
	{
		//3.�����·���ȯ�ϰ�ʹ�
		mState = EEnemyState::Idle;
		//����ð��ʱ�ȭ
		currentTime = 0;
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
	}
}
//��������
void UEnemyFSM::DieState()
{

}

//�ǰݾ˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess()
{
	//ü�°��� 
	hp--;
	//���࿡ ü���� �����ִٸ� 
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;
	}
	//�׷���������
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
	}
	//�ִϸ��̼ǻ��� ����ȭ
	anim->animState = mState;
}

