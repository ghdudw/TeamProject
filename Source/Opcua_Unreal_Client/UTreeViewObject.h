// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * 
 */
class OPCUA_UNREAL_CLIENT_API UTreeViewObject
{
public:
    UPROPERTY()
    UTreeViewObject* Parent;

    UPROPERTY()
    TArray<UTreeViewObject*> Children;

    UPROPERTY()
    FString Name; // ��� �̸�

    UTreeViewObject()
    {
        Parent = nullptr;
    }
};
