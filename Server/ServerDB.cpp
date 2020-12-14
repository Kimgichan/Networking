// ServerDB.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <ctime>
#include <chrono>
#include "DataEncoding.h"


#pragma comment(lib, "ws2_32.lib")

using namespace std;

const clock_t LIFE_CYCLE = 10 * CLOCKS_PER_SEC;
const int BUFFER_SIZE = 36;

class SocketInfo {
private:
    SOCKET socket;
    clock_t heartbeat;
    MYSQL* connection;
    MYSQL conn;
    bool run;  
    DataEncoding* dbEncoding;
    DB_PROJECT DB_project;

public:
    SocketInfo(SOCKET _socket, bool _run) {
        socket = _socket;
        dbEncoding = nullptr;
        run = _run;
        cout << "시작 시간: "<<heartbeat << endl;
        heartbeat = clock();
        DB_project = DB_PROJECT::DB_NULL;
        this->connection = NULL; 
        conn;
        mysql_init(&conn);
        this->connection = mysql_real_connect(&conn, "localhost", "root", "Kim3509!", "new_schema", 3306, (char*)NULL, 0);
        if (this->connection == NULL) {
            cout << "DB error" << endl;
            return;
        }

        //쿼리:한글사용위해 
        mysql_query(this->connection, "set session character_set_connection=euckr;");
        mysql_query(this->connection, "set session character_set_results=euckr;");
        mysql_query(this->connection, "set session character_set_client=euckr;");
    }

    static void updateFunc(vector<SocketInfo*>& _array) {
        clock_t timer;
        while (true) {
            timer = clock();
            for (int indx = 0; indx < _array.size(); indx++) {
                if ((timer - (_array[indx]->heartbeat) > LIFE_CYCLE)) {
                    //cout << execution <<" - "<< _array[indx]->heartbeat<< endl;
                    //cout << execution - _array[indx]->heartbeat <<" > "<< LIFE_CYCLE << " ?"<< endl;
                    //cout << "number : " << _array[indx]->id << endl;
                    _array[indx]->run = false;
                    _array.erase(_array.begin() + indx);
                    indx--;
                }
                //if (execution - (*iter)->heartbeat > LIFE_CYCLE) {
                //    (*iter)->run = false;
                //    iter = _array.erase(iter);
                //    cout << "뭐여?" << endl;
                //}
                //else {
                //    iter++;
                //}
            }
            this_thread::sleep_for(0.1s);
        }
    }

    static void responseFunc(SocketInfo* _socketInfo) {
        char buffer[BUFFER_SIZE];
        while (_socketInfo->run) {
            if (recv(_socketInfo->socket, buffer, sizeof(buffer), 0) != SOCKET_ERROR) {
                if (_socketInfo->run) {
                    _socketInfo->heartbeat = clock();
                    int query = *((int*)(buffer));
                    cout << query << endl;
                    //쿼리문
                    if (_socketInfo->DB_project == DB_PROJECT::DB_NULL) {
                        if (query == QUERY::Q_USER) {
                            cout << "???" << endl;
                            _socketInfo->DB_project = DataEncoding::PROJECT_TABLE[(wstring((wchar_t*)(&buffer[4])))];
                            _socketInfo->dbEncoding = DataEncoding::SelectEncoding(_socketInfo->DB_project);
                            send(_socketInfo->socket, buffer, BUFFER_SIZE, 0);
                        }
                    }
                    else {
                        switch (query) {
                            case QUERY::Q_ADD: {
                                string sendbuffer=_socketInfo->dbEncoding->Add(buffer, _socketInfo->connection);
                                send(_socketInfo->socket, sendbuffer.c_str(), BUFFER_SIZE, 0);
                            } break;

                            case QUERY::Q_DELETE: {
                                string sendbuffer = _socketInfo->dbEncoding->Erase(buffer, _socketInfo->connection);
                                send(_socketInfo->socket, sendbuffer.c_str(), BUFFER_SIZE, 0);
                            } break;

                            case QUERY::Q_SERCH_DATA: {
                                cout << "serch" << endl;
                                string sendbuffer = _socketInfo->dbEncoding->SearchData(buffer, _socketInfo->connection);
                                send(_socketInfo->socket, sendbuffer.c_str(), BUFFER_SIZE, 0);
                            } break;

                            case QUERY::Q_SERCH_NAME: {
                                string sendbuffer = _socketInfo->dbEncoding->SearchData(buffer, _socketInfo->connection);
                                send(_socketInfo->socket, sendbuffer.c_str(), BUFFER_SIZE, 0);
                            } break;
                        }
                    }
                }
            }
        }
        closesocket(_socketInfo->socket);
        cout << "끝" << endl;
        delete _socketInfo;
    }
};

int main()
{
    DataEncoding::PROJECT_TABLE[L"ProjectA"] = DB_PROJECT::DB_PROJECT_A;


    WSADATA wsaData;
    SOCKET hSocket;

    //윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == INVALID_SOCKET) {
        cout << "Failed WSAStartup()" << endl;
        return 1;
    }

    //socket 생성
    hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == INVALID_SOCKET) {
        cout << "Failed socket()" << endl;
        return 1;
    }


    //서버 자신의 주소를 셋팅
    sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.30.1.20");
    addr.sin_port = htons(8000);

    int socktime = 3000; //약 3초
    setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&socktime, sizeof(socktime));

    if (bind(hSocket, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "바인딩 실패" << endl;
        return 1;
    }
    if (listen(hSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "듣기 실패" << endl;
        return 1;
    }

    vector<SocketInfo*> clients;
    thread(SocketInfo::updateFunc, ref(clients)).detach();

    while (true) {
        SocketInfo* client = new SocketInfo(accept(hSocket, NULL, NULL),true);
        clients.push_back(client);
        //thread 문 추가
        thread(SocketInfo::responseFunc, clients.back()).detach();
    }

    //closesocket(hSocket);
    WSACleanup();
    cout << "종료된다고?" << endl;
    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
