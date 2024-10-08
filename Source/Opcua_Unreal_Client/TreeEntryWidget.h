// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UTreeViewObject.h"
#include "TreeEntryWidget.generated.h"
/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API UTreeEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	void OnItemSelected(UTreeViewObject* SelectedItem);

	TSharedRef<ITableRow> OnGenerateRow(UTreeViewObject* InItem, const TSharedRef<STableViewBase>& OwnerTable);
	
};
