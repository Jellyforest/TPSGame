// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingWidget.h"
#include "Components/TextBlock.h"
#include "WorldWarGameMode.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_Quit->OnClicked.AddDynamic(this, &UEndingWidget::QuitGame);
	btn_EnterName->OnClicked.AddDynamic(this, &UEndingWidget::EnterName);
}

void UEndingWidget::PrintCurrentScore()
{
	AWorldWarGameMode* myGM = Cast<AWorldWarGameMode>(UGameplayStatics::GetGameMode(this));

	if (myGM != nullptr)
	{
		// ���� ����(����) -> FText (���ڿ�) ���·� ��ȯ�Ѵ�.
		FText scoreText = FText::AsNumber(myGM->GetCurrentScore());

		//curScore �ؽ�Ʈ ����� ������ �����Ѵ�.
		curScore->SetText(scoreText);

		// �������� ���
		FText recordFirstText = FText::AsNumber(myGM->scoreFirst);
		FText recordSecondText = FText::AsNumber(myGM->scoreSecond);
		FText recordThirdText = FText::AsNumber(myGM->scoreThird);
		recordFirst->SetText(recordFirstText);
		recordSecond->SetText(recordSecondText);
		recordThird->SetText(recordThirdText);

		// ������ �̸� ���
		recordFirstName->SetText(FText::FromString(myGM->scoreFirstName));
		recordSecondName->SetText(FText::FromString(myGM->scoreSecondName));
		recordThirdName->SetText(FText::FromString(myGM->scoreThirdName));

	}
}

void UEndingWidget::EnterName()
{
	AWorldWarGameMode* myGM = Cast<AWorldWarGameMode>(UGameplayStatics::GetGameMode(this));
	myGM->currentName =  InputText->GetText().ToString();
	myGM->RecordScore();
	myGM->SaveScore();
	if (myGM->ending_UI)
	{
		myGM->ending_UI->PrintCurrentScore();
	}

	InputText->SetVisibility(ESlateVisibility::Hidden);
	btn_EnterName->SetVisibility(ESlateVisibility::Hidden);
}

void UEndingWidget::QuitGame()
{
	//���� �����Ѵ�
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playerCon, EQuitPreference::Quit, true);
}
