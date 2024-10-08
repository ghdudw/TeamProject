// Fill out your copyright notice in the Description page of Project Settings.


#include "SEntryListWidget.h"
#include "Components/TextBlock.h"
#include "MyCustomStruct.h"
#include "UListViewObject.h"
#include <Misc/DefaultValueHelper.h>

void USEntryListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (const UUListViewObject* ListItem = Cast<UUListViewObject>(ListItemObject))
    {
        switch (ListItem->StructType)
        {
        case 1:
            // ù ��° ����ü ������ ó��
            DataText_1->SetText(FText::FromString(ListItem->MyLevelStruct.DisplayName));
            DataText_2->SetText(FText::AsNumber(ListItem->MyLevelStruct.ValueDouble));
            break;
        case 2:
            // �� ��° ����ü ������ ó��
            DataText_1->SetText(FText::FromString(ListItem->MySwitchStruct.DisplayName));
            DataText_2->SetText(FText::FromString(ListItem->MySwitchStruct.ValueBool ? "True" : "False"));
            break;
        case 3:
            // �� ��° ����ü ������ ó��
        {
            DataText_1->SetText(FText::FromString(ListItem->EventIdStruct.DisplayName));

            TArray<uint8> ByteArray = ListItem->EventIdStruct.ValueByteString;

            // �� ��Ҹ� ���ڿ��� ��ȯ�Ͽ� �ϳ��� FString���� ����
            FString ByteString;
            for (uint8 ByteValue : ByteArray)
            {
                ByteString += FString::FromInt(ByteValue) + TEXT(" ");  // �� �� ���̿� ���� �߰�
            }

            // ���յ� FString�� FText�� ��ȯ�Ͽ� SetText�� ����
            DataText_2->SetText(FText::FromString(ByteString));
            break;
        }
        case 4:
            // �� ��° ����ü ������ ó��
            DataText_1->SetText(FText::FromString(ListItem->ReciveTimeStruct.DisplayName));
            DataText_2->SetText(FText::AsDateTime(ListItem->ReciveTimeStruct.ValueDateTime));
            break;
        case 5:
            // �ټ� ��° ����ü ������ ó��
            DataText_1->SetText(FText::FromString(ListItem->SeverityStruct.DisplayName));
            DataText_2->SetText(FText::AsNumber(ListItem->SeverityStruct.ValueInt32));
            break;
        default:
            break;
        }
    }
}
