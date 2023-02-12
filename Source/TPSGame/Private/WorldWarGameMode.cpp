// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarGameMode.h"

#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EndingWidget.h"
#include "StartWidget.h"
#include "BGMManager.h"
#include "EngineUtils.h"

void AWorldWarGameMode::BeginPlay()
{
	Super::BeginPlay();

	start_UI = CreateWidget<UStartWidget>(GetWorld(), startWidget);
	over_UI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget);
	ending_UI = CreateWidget<UEndingWidget>(GetWorld(), endingwWidget);
	
	if (start_UI != nullptr)
	{
		//������ ������ ����Ʈ�� �׸���.

		start_UI->AddToViewport();
	}

	//���콺 Ŀ���� ȭ�鿡 ���̰� �Ѵ�
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	for(auto actor : TActorRange<ABGMManager>(GetWorld()))
	{
		bgm = actor;
	}
}

void AWorldWarGameMode::ShowGameOver()
{

	if(over_UI != nullptr)
	{
		over_UI->AddToViewport();
	}

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	// ������� ���
	bgm->PlayBGMDie();
	// ī��Ʈ�ٿ� Ÿ�̸� ���
	GetWorldTimerManager().ClearTimer(countdownHandle);
}

void AWorldWarGameMode::ShowEnding()
{
	if (ending_UI != nullptr)
	{
		ending_UI->AddToViewport();
	}
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	// ������� ���
	bgm->PlayBGMClear();
	// ī��Ʈ�ٿ� Ÿ�̸� ���
	GetWorldTimerManager().ClearTimer(countdownHandle);
}

void AWorldWarGameMode::CountdownTimer(int time)
{
	currentCountdown = time;
	GetWorldTimerManager().SetTimer(countdownHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (currentCountdown > 0)
			{
				currentCountdown--;
				if (currentCountdown == 1 && stage == 2)
				{
					UGameplayStatics::SetGlobalTimeDilation(this, 0.3);
				}
			}
			else
			{
				currentCountdown = 0;
				bCanSpawnZombie = !bCanSpawnZombie;
				GetWorldTimerManager().ClearTimer(countdownHandle);
			}
		}), 1.f, true);
}

