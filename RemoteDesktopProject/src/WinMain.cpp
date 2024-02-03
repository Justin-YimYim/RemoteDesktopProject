#include <iostream>
#include "pch.hpp"
#include "WinSockLib.hpp"
#include "Network/TCPInterface.hpp"
#include "Window/WindowInterface.hpp"

using namespace std;

int WINAPI wWinMain(_In_   HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR lpCmdLine,
    _In_     int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Example of using Socket
    /*
    RemoteDesk::WinsockLib winsockLib{};

    RemoteDesk::TCPSocket socket{};

    auto results = RemoteDesk::TCPResolver::Resolve("google.com", "80");
    auto endpoint = results.GetEndpoint();

    std::vector<std::byte> pageData{};
    pageData.resize(1024 * 1024 * 20);

    RemoteDesk::TCPStream stream(socket);

    std::string request{ "GET / HTTP/1.0\r\nHost: google.com\r\nUser-Agent: RemoteDesk\r\n\r\n" };

    try {
        if (!socket.Connect(endpoint, std::nothrow)) throw std::runtime_error("Cannot connect");
        if (socket.Send(request.data(), request.size(), std::nothrow) == SOCKET_ERROR) throw std::runtime_error("Cannot Send");

        stream.Receive(pageData);
    }catch(exception& ex){
        MessageBoxA(nullptr, ex.what(), "Exception Throw", MB_OK);
    }


    std::ofstream file("file.txt", std::ios::binary);
    file.write((char*)pageData.data(), 1024 * 20);*/

    RemoteDesk::SelectionWindow window{};

    MSG msg{};

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (static_cast<int>(msg.wParam) == 0) {
        RemoteDesk::SelectionWindow window{};

        MSG msg{};

        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}