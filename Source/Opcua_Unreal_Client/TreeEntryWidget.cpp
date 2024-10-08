// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeEntryWidget.h"

void UTreeEntryWidget::OnItemSelected(UTreeViewObject* SelectedItem)
{
    // 선택한 아이템의 데이터 처리
    UE_LOG(LogTemp, Log, TEXT("Selected Item: %s"), *SelectedItem->Name);
}

TSharedRef<ITableRow> UTreeEntryWidget::OnGenerateRow(UTreeViewObject* InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<UTreeViewObject*>, OwnerTable)
        [
            SNew(STextBlock)
                .Text(FText::FromString(InItem->Name))
        ];
}
