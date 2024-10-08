// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TCP_IP_ClientWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API UTCP_IP_ClientWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TCPData_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TCPData_2;

private:
    void ConnectToServer();
    void ReceiveJsonData(const FString& JsonString);
    void ReceiveData();
    void LogParsedData();
    void CloseSocket();

    FSocket* Socket;

    // ParsedData�� MyActor.cpp���� ���ǵ�
    static FMyStruct ParsedData;  // ��� ���Ͽ��� static���� ����

    // Ÿ�̸� �ڵ鷯
    FTimerHandle TimerHandle;
};

USTRUCT()
struct FMyStruct
{
    GENERATED_BODY()

    UPROPERTY()
    double TemperatureValue;

    UPROPERTY()
    FString State;
};