// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"


/**
 * 
 */
UCLASS()
class TPSGAME_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=FSM)
	EEnemyState animState;

	//���ݻ���������� ����
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category=FSM)
	bool bAttackPlay = false;

	//���� �ִϸ��̼��� ������ �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
	void PlayDamageAnim(FName sectionName);

	UFUNCTION()
	void AnimNotify_EnemyAttackStart();

	UFUNCTION()
	void AnimNotify_EnemyAttackEnd();

	class AEnemy* me;


};
