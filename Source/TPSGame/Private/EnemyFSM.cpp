// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include "AIController.h"
#include "MyPlayer.h"
#include "EnemyAnim.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"
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
	//AMyPlayerŸ������ĳ����
	target = Cast<AMyPlayer>(actor);
	//������ü��������
	me = Cast<AEnemy>(GetOwner());

	//UEnemyAnim*�Ҵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	//AAIController �Ҵ��ϱ�
	ai = Cast<AAIController>(me->GetController());

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
		AttackState();
		break;
	case EEnemyState::Damage:
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
 	if (!canMove)
	{
		return;
 	}
	//Ÿ�� ������
	FVector destination = target->GetActorLocation();
	// ����
	FVector dir = destination - me->GetActorLocation();

	//�������� �̵��ϰ�ʹ�
	//me->AddMovementInput(dir.GetSafeNormal());
	ai->MoveToLocation(destination);


/*
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;

	FAIMoveRequest req;

	req.SetAcceptanceRadius(100);

	req.SetGoalLocation(destination);

	ai->BuildPathfindingQuery(req, query);

	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	*/

	//Ÿ��� ��������� ���ݻ��·� ��ȯ�ϰ�ʹ�
	//1. ���� �Ÿ��� ���ݹ����ȿ� ������
	if (dir.Size() < attackRange)
	{
		ai->StopMovement();
		//2.���ݻ��·� ��ȯ�ϰ�ʹ�
		mState = EEnemyState::Attack;
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		//���ݾִϸ��̼� ���Ȱ��ȭ
		anim->bAttackPlay = true;
		//���ݻ�����ȯ�� ���ð��� �ٷ� �������� ó��
		currentTime = attackDelayTime;

	}
	


	//�����Ǥ��� 
}
//���ݻ���
void UEnemyFSM::AttackState()
{
	

	    //��ǥ: ���� �ð��� �ѹ��� �����ϰ�ʹ�
	    //1. �ð��� �귯���Ѵ�
	    currentTime += GetWorld()->DeltaTimeSeconds;
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
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > dieDelayTime)
	{
		//��� �Ʒ��� ��������ʹ�
		//��ӿ���� P = P0+vt
		FVector P0 = me->GetActorLocation();
		FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
		FVector P = P0 + vt;
		me->SetActorLocation(P);

		//����2�����̻󳻷��Դٸ�
		if (P.Z < -200.0f)
		{
			//���Ž�Ų��
			me->Destroy();
		}

	}
}

//�ǰݾ˸� �̺�Ʈ �Լ�
void UEnemyFSM::OnDamageProcess(int val)
{
	UE_LOG(LogTemp, Warning, TEXT("enemy hited"));
	//ü�°��� 
	hp -= val;
	//���࿡ ü���� �����ִٸ� 
	if (hp > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("?"));
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;

		currentTime = 0;
		//�ǰݾִϸ��̼� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));

	}
	//�׷���������
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("zugum"));
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//���� �ִϸ��̼� ���
		anim->PlayDamageAnim(TEXT("Die"));

	}

	//�ִϸ��̼ǻ��� ����ȭ
	anim->animState = mState;

	ai->StopMovement();
}

