// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingWidget.h"
#include "Components/TextBlock.h"
#include "WorldWarGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_Quit->OnClicked.AddDynamic(this, &UEndingWidget::QuitGame);
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
	}
}

void UEndingWidget::QuitGame()
{
	//���� �����Ѵ�
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playerCon, EQuitPreference::Quit, true);
}
