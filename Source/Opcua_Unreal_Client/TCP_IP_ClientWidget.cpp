// Fill out your copyright notice in the Description page of Project Settings.


#include "TCP_IP_ClientWidget.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Components/TextBlock.h"

// ���� ���� �ʱ�ȭ
FMyStruct UTCP_IP_ClientWidget::ParsedData;

void UTCP_IP_ClientWidget::NativeConstruct()
{
    Super::NativeConstruct();
	Socket = nullptr;
    ConnectToServer();
}

void UTCP_IP_ClientWidget::NativeDestruct()
{
    Super::NativeDestruct();
    CloseSocket();
}

void UTCP_IP_ClientWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    ReceiveData();  // �� �����Ӹ��� ������ ���� Ȯ��
}

void UTCP_IP_ClientWidget::ConnectToServer()
{
    FString ServerAddress = TEXT("127.0.0.1"); // ���� IP
    int32 Port = 10990; // ��Ʈ ��ȣ
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);

    // ���� �ּ� ����
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
    bool bIsValid;
    Addr->SetIp(*ServerAddress, bIsValid);
    Addr->SetPort(Port);

    // IP �ּ� ��ȿ�� Ȯ��
    if (!bIsValid)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid IP address"));
        return;
    }

    // ���� ����
    Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("DefaultSocket"), false);
    if (Socket == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create socket"));
        return;
    }

    // ������ ���� �õ�
    bool bConnected = Socket->Connect(*Addr);
    if (bConnected)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully connected to server"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
    }
}

void UTCP_IP_ClientWidget::ReceiveJsonData(const FString& JsonString)
{
    // JSON �Ľ�
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    // JSON �Ľ� ���� ���� Ȯ��
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
    {
        // "MyStruct" ��ü ��������
        TSharedPtr<FJsonObject> MyStruct = JsonObject->GetObjectField(TEXT("MyStruct"));
        if (MyStruct.IsValid())  // MyStruct ��ȿ�� Ȯ��
        {
            // State ���� ������ ParsedData�� ����
            ParsedData.State = MyStruct->GetStringField(TEXT("State"));
            // Temperature ���� �� ��������
            ParsedData.TemperatureValue = MyStruct->GetNumberField(TEXT("Temperature"));

            // State�� Temperature ���� �α� ���
            LogParsedData();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MyStruct is invalid or missing"));
        }
    }
    else
    {
        // JSON �Ľ� ���� �� �� ���� ���� ����
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON: %s"), *JsonString);
    }
}

void UTCP_IP_ClientWidget::ReceiveData()
{
    if (Socket && Socket->GetConnectionState() == SCS_Connected)
    {
        uint32 DataSize;
        if (Socket->HasPendingData(DataSize))
        {
            TArray<uint8> ReceivedData;
            ReceivedData.SetNumUninitialized(DataSize);

            int32 BytesRead = 0;
            if (Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead))
            {
                // ���ŵ� �����͸� ���ڿ��� ��ȯ
                FString ReceivedString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));

                // ������ ���ڿ� ����
                if (ReceivedString.EndsWith(TEXT("\n"))) {
                    ReceivedString = ReceivedString.Left(ReceivedString.Len() - 1);
                }

                // �߰� ���� (��: Ư�� Ư������ ����)
                ReceivedString.RemoveFromEnd(TEXT("\r")); // ĳ���� ���� ����

                // ������ ���ڿ��� JSON���� ó��
                ReceiveJsonData(ReceivedString);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to read from socket"));
            }
        }
    }
}

void UTCP_IP_ClientWidget::LogParsedData()
{
	// State�� Temperature ���� �α� ���
	UE_LOG(LogTemp, Log, TEXT("State: %s"), *ParsedData.State);
	UE_LOG(LogTemp, Log, TEXT("Temperature: %.1f"), ParsedData.TemperatureValue);

    TCPData_1->SetText(FText::FromString(*ParsedData.State));
    TCPData_2->SetText(FText::AsNumber(ParsedData.TemperatureValue));
}

void UTCP_IP_ClientWidget::CloseSocket()
{
    // ClientSocket�� nullptr�� �ƴϰ�, ��ȿ�� ���� Ÿ������ Ȯ��
    if (Socket != nullptr && Socket->GetSocketType() != ESocketType::SOCKTYPE_Unknown)
    {
        Socket->Close(); // ���� �ݱ�
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket); // ���� ���ҽ� ����
        Socket = nullptr; // ���� �����͸� null�� �����Ͽ� �޸� ���� ����
        UE_LOG(LogTemp, Log, TEXT("Socket closed successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Socket is already closed or not initialized."));
    }
}
