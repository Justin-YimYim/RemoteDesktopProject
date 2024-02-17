#pragma once

#define _WIN32_WINNT 0x0A00

#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <d3d11.h>
#include <dxgi1_5.h>
#include <d2d1_1.h>

#include <DirectXColors.h>

#include <wrl/client.h>

#include <stdexcept>
#include <system_error>
#include <tuple>
#include <vector>
#include <type_traits>
#include <string>
#include <thread>
#include <new>
#include <fstream>
