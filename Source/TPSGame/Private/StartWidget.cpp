// Fill out your copyright notice in the Description page of Project Settings.

#include "StartWidget.h"
#include "kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"



void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Start->OnClicked.AddDynamic(this, &UStartWidget::StartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UStartWidget::QuitGame);
}

void UStartWidget::StartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	this->RemoveFromParent();

	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));

	//���� ������ �����Ѵ�
	//this->RemoveFromParent();

}

void UStartWidget::QuitGame()
{
	//���� �����Ѵ�
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playerCon, EQuitPreference::Quit, true);
}
