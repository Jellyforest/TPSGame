// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_ReStart->OnClicked.AddDynamic(this,&UGameOverWidget::ReStartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGame);
}

void UGameOverWidget::ReStartGame()
{
	//������ �ٽ� �ε��Ѵ� 
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));

	//���� ������ �����Ѵ�
	this->RemoveFromParent();
}

void UGameOverWidget::QuitGame()
{
	//���� �����Ѵ�
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playerCon, EQuitPreference::Quit, true);
}
