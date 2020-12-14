// Fill out your copyright notice in the Description page of Project Settings.
#include "Engine.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "ClientSocket.h"
#include <cstring>

using namespace std;
// Sets default values
AClientSocket::AClientSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	socket = nullptr;
	recv_thread = nullptr;
	connected = false;
}


void AClientSocket::Connect(UPARAM(ref)FString IP, int32 Port, int32 ConnectBuffer)
{
	if (!connected) {
		conn_thread = FRunnableThread::Create(new CSConnectThread(*this, IP, Port, ConnectBuffer), TEXT("ConnectThread"));
	}
}

void AClientSocket::Deconnect()
{
	if (connected) {


		socket->Close();
		socket = nullptr;
		connected = false;
	}

	if (recv_thread != nullptr) {
		recv_thread->Kill(true);
		delete recv_thread;
		recv_thread = nullptr;
	}
}

void AClientSocket::SendMSG(UPARAM(ref)TArray<uint8> msg)
{
	if (connected) {
		int32 read = 0;
		socket->Send(msg.GetData(), msg.Num(), read);
	}
}

int32 AClientSocket::GetBuffer(AClientSocket* client_socket)
{
	return client_socket->buffer.Num();
}

// Called when the game starts or when spawned
void AClientSocket::BeginPlay()
{
	Super::BeginPlay();


	//
	//socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	//FIPv4Address ip(14, 39, 113, 172);
	//TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//addr->SetIp(ip.Value);
	//addr->SetPort(8000);

	//
	////FString cc = TEXT("생명 연장 시켜줘 ㅠㅠ");
	//if (socket->Connect(*addr)) {
	//	UE_LOG(LogTemp, Log, TEXT("success"));
	//	//int read = 0;
	//	//socket->Send((uint8*)(cc.GetCharArray().GetData()), 1024, read);
	//}
	//else {
	//	UE_LOG(LogTemp, Log, TEXT("fail"));
	//}
	////
	//timer = clock();
}

void AClientSocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Deconnect();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AClientSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//clock_t t = clock();
	//if (t - timer > life_cycle) {
	//	FString cc = TEXT("생명 연장 시켜줘 ㅠㅠ");
	//	int read = 0;
	//	socket->Send((uint8*)(cc.GetCharArray().GetData()), 1024, read);
	//	timer = clock();
	//}
}

CSRecvThread::CSRecvThread(AClientSocket& _user): user(&_user)
{
	run = true;
}

CSRecvThread::~CSRecvThread()
{
	run = false;
}

uint32 CSRecvThread::Run()
{
	
	while (run) {
		int32 read = 0;
		FSocket* socket = user->socket;
		if (user->socket != nullptr) {
			if (user->socket->Recv(user->buffer.GetData(), user->buffer.Num(), read)) {
				//UE_LOG(LogTemp, Log, TEXT("fail"))
				if (user->socket != nullptr) {
					user->RecvMSG(user->buffer);
				}
			}
		}	
		FPlatformProcess::Sleep(0.01f);
	}
	return 1;
}

void CSRecvThread::Stop()
{
	run = false;
}

CSConnectThread::CSConnectThread(AClientSocket& _user, FString& _IP, int32 _Port, int32 _ConnectBuffer)
{
	user = &_user;
	IP = &_IP;
	Port = _Port;
	ConnectBuffer = _ConnectBuffer;
}

CSConnectThread::~CSConnectThread()
{
}

uint32 CSConnectThread::Run()
{
	user->socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	FIPv4Address ip;
	FIPv4Address::Parse(*IP, ip);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(Port);
	if (user->socket->Connect(*addr)) {
		user->buffer.Empty();
		user->buffer.Init(0, ConnectBuffer);
		user->recv_thread = FRunnableThread::Create(new CSRecvThread(*user), TEXT("RecvThread"));
		user->connected = true;
	}
	return uint32(1);
}

void CSConnectThread::Stop()
{
}
