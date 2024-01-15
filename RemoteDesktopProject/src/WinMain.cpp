#include <iostream>
#include "pch.h"
#include "WinSockLib.h"
#include "SafeSocket.h"
#include "TCPSocket.h"

using namespace std;

int WINAPI wWinMain(_In_   HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_     LPWSTR lpCmdLine,
                    _In_     int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance); 
    UNREFERENCED_PARAMETER(lpCmdLine);

    RemoteDesk::WinsockLib winsockLib{};

    RemoteDesk::TCPSocket listener{};

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2302);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    listener.Bind(addr);
    listener.Listen();


    std::jthread clientThread([]() 
        {
            RemoteDesk::TCPSocket socket{};
        
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(2302);
            addr.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);

            socket.Connect(addr);

            std::string message = "Hello World";
            socket.Send(message);

            message = socket.Receive(10);
            if (message.empty()) {
                MessageBoxA(nullptr, "Disconnected", "", MB_OK);
            }
        });

    RemoteDesk::TCPSocket client = listener.Accept();
    std::string message = client.Receive(12);
    MessageBoxA(nullptr, message.c_str(), "Message!", MB_OK);

    client.ShutDown();

    return 0;
}