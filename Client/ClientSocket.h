// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HAL/Runnable.h"
#include "ClientSocket.generated.h"


UCLASS()
class SOCKET_TEST_API AClientSocket : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClientSocket();

	//
	UFUNCTION(BlueprintCallable, Category = "ClientSocket")
		void Connect(UPARAM(ref) FString IP, int32 Port, int32 ConnectBuffer);
	UFUNCTION(BlueprintCallable, Category = "ClientSocket")
		void Deconnect();
	UFUNCTION(BlueprintCallable, Category = "ClientSocket")
		void SendMSG(UPARAM(ref) TArray<uint8> msg);
	UFUNCTION(BlueprintPure, Category = "ClientSocket")
		static int32 GetBuffer(AClientSocket* client_socket);
	UFUNCTION(BlueprintImplementableEvent, Category = "ClientSocket")
		void RecvMSG(const TArray<uint8>& msg);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//

	FSocket* socket;
	FRunnableThread* recv_thread;
	FRunnableThread* conn_thread;
	TArray<uint8> buffer;
	bool connected;
};

class CSRecvThread : public FRunnable {
private:
	AClientSocket* user;
	bool run;


public:
	CSRecvThread(AClientSocket& _user);
	~CSRecvThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
};

class CSConnectThread : public FRunnable {
private:
	AClientSocket* user;
	FString* IP;
	int32 Port;
	int32 ConnectBuffer;

public:
	CSConnectThread(AClientSocket& _user, FString& _IP, int32 _Port, int32 _ConnectBuffer);
	~CSConnectThread();

	virtual uint32 Run() override;
	virtual void Stop() override;
};



