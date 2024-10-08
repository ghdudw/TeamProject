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

    // JSON 데이터 로드 (예제 JSON 문자열)
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
        // 현재 가시성 상태를 가져와서 토글 (보이면 숨기고, 숨기면 보이도록)
        ESlateVisibility CurrentVisibility = BP_ShutDownWidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // 가시성이 보이는 상태라면 숨기기
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // 숨겨진 상태라면 보이게 하기
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::OnClickedOption()
{
    if (BP_optionwidget)
    {
        // 현재 가시성 상태를 가져와서 토글 (보이면 숨기고, 숨기면 보이도록)
        ESlateVisibility CurrentVisibility = BP_optionwidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // 가시성이 보이는 상태라면 숨기기
            BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // 숨겨진 상태라면 보이게 하기
            BP_optionwidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::UpdateDateTime()
{
    if (TextBlock_Timer)
    {
        // 현재 시간을 가져와서 텍스트 블록에 설정
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

    // 위치 정보 추가
    UTreeViewObject* LocationItem = NewObject<UTreeViewObject>();
    LocationItem->Name = "Location: " + JsonObject->GetStringField("location");
    LocationItem->Parent = LibraryItem;

    LibraryItem->Children.Add(LocationItem);

    // 책 정보 추가
    TArray<TSharedPtr<FJsonValue>> BooksArray = JsonObject->GetArrayField("books");
    for (const TSharedPtr<FJsonValue>& BookValue : BooksArray)
    {
        TSharedPtr<FJsonObject> BookObject = BookValue->AsObject();

        UTreeViewObject* BookItem = NewObject<UTreeViewObject>();
        BookItem->Name = BookObject->GetStringField("title");

        // 저자 정보 추가
        TSharedPtr<FJsonObject> AuthorObject = BookObject->GetObjectField("author");
        UTreeViewObject* AuthorItem = NewObject<UTreeViewObject>();
        AuthorItem->Name = "Author: " + AuthorObject->GetStringField("name");
        BookItem->Children.Add(AuthorItem);

        // 도서 추가
        LibraryItem->Children.Add(BookItem);

    }
    // 트리 뷰에 추가
    
   // MyTreeView->AddItem(LibraryItem));
}

FText USMainWidget::GetCurrentDateTime() const
{
    FDateTime Now = FDateTime::Now();

    // 날짜와 시간을 문자열로 변환
    FString DateTimeString = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    // 텍스트로 변환 후 반환
    return FText::FromString(DateTimeString);
}