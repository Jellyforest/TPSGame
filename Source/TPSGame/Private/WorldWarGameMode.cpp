// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWarGameMode.h"

#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"

#include "StartWidget.h"

void AWorldWarGameMode::BeginPlay()
{
	Super::BeginPlay();

	start_UI = CreateWidget<UStartWidget>(GetWorld(), startWidget);

	if (start_UI != nullptr)
	{
		//������ ������ ����Ʈ�� �׸���.

		start_UI->AddToViewport();
	}

	//���콺 Ŀ���� ȭ�鿡 ���̰� �Ѵ�
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
}

void AWorldWarGameMode::ShowGameOver()
{
	//���� ������ ȭ�鿡 ����
	over_UI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget);

	if(over_UI != nullptr)
	{
		over_UI->AddToViewport();
	}
	
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}


