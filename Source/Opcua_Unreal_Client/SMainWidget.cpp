// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainWidget.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"
#include "MyCustomStruct.h"
#include "MyPlayerController.h"
#include "Components/ListView.h"
#include "UListViewObject.h"
#include "SEntryListWidget.h"
#include "Blueprint/UserWidget.h"
#include "FindInBlueprints.h"
#include "UTreeViewObject.h"
#include "Components/TreeView.h"


void USMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // JSON ������ �ε� (���� JSON ���ڿ�)
    FString JsonString = TEXT("{ \"library\": { \"name\": \"City Library\", \"location\": \"Downtown\", \"books\": [ { \"title\": \"The Great Gatsby\", \"author\": { \"name\": \"F. Scott Fitzgerald\", \"birthYear\": 1896 } }, { \"title\": \"1984\", \"author\": { \"name\": \"George Orwell\", \"birthYear\": 1903 } } ] } } }");

    LoadJsonData(JsonString);

    BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
    BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);


    Off->OnClicked.AddDynamic(this, &USMainWidget::OnClickedOff);
    option->OnClicked.AddDynamic(this, &USMainWidget::OnClickedOption);
    //home->OnClicked.AddDynamic(this, &USMyUserWidget::OnResetButtonClicked);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USMainWidget::UpdateDateTime, 1.0f, true);

    UpdateDateTime();

}

void USMainWidget::OnClickedOff()
{
    if (BP_ShutDownWidget)
    {
        // ���� ���ü� ���¸� �����ͼ� ��� (���̸� �����, ����� ���̵���)
        ESlateVisibility CurrentVisibility = BP_ShutDownWidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // ���ü��� ���̴� ���¶�� �����
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // ������ ���¶�� ���̰� �ϱ�
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::OnClickedOption()
{
    if (BP_optionwidget)
    {
        // ���� ���ü� ���¸� �����ͼ� ��� (���̸� �����, ����� ���̵���)
        ESlateVisibility CurrentVisibility = BP_optionwidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // ���ü��� ���̴� ���¶�� �����
            BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // ������ ���¶�� ���̰� �ϱ�
            BP_optionwidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::UpdateDateTime()
{
    if (TextBlock_Timer)
    {
        // ���� �ð��� �����ͼ� �ؽ�Ʈ ��Ͽ� ����
        TextBlock_Timer->SetText(GetCurrentDateTime());
    }

}

void USMainWidget::LoadJsonData(FString JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        CreateTreeFromJson(JsonObject);
    }
}

void USMainWidget::CreateTreeFromJson(TSharedPtr<FJsonObject> JsonObject)
{
    UTreeViewObject* LibraryItem = NewObject<UTreeViewObject>();
    LibraryItem->Name = JsonObject->GetStringField("name");

    // ��ġ ���� �߰�
    UTreeViewObject* LocationItem = NewObject<UTreeViewObject>();
    LocationItem->Name = "Location: " + JsonObject->GetStringField("location");
    LocationItem->Parent = LibraryItem;

    LibraryItem->Children.Add(LocationItem);

    // å ���� �߰�
    TArray<TSharedPtr<FJsonValue>> BooksArray = JsonObject->GetArrayField("books");
    for (const TSharedPtr<FJsonValue>& BookValue : BooksArray)
    {
        TSharedPtr<FJsonObject> BookObject = BookValue->AsObject();

        UTreeViewObject* BookItem = NewObject<UTreeViewObject>();
        BookItem->Name = BookObject->GetStringField("title");

        // ���� ���� �߰�
        TSharedPtr<FJsonObject> AuthorObject = BookObject->GetObjectField("author");
        UTreeViewObject* AuthorItem = NewObject<UTreeViewObject>();
        AuthorItem->Name = "Author: " + AuthorObject->GetStringField("name");
        BookItem->Children.Add(AuthorItem);

        // ���� �߰�
        LibraryItem->Children.Add(BookItem);

    }
    // Ʈ�� �信 �߰�
    
   // MyTreeView->AddItem(LibraryItem));
}

FText USMainWidget::GetCurrentDateTime() const
{
    FDateTime Now = FDateTime::Now();

    // ��¥�� �ð��� ���ڿ��� ��ȯ
    FString DateTimeString = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    // �ؽ�Ʈ�� ��ȯ �� ��ȯ
    return FText::FromString(DateTimeString);
}