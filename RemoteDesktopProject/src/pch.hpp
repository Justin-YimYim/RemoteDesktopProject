#pragma once

#define _WIN32_WINNT 0x0A00

#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <stdexcept>
#include <system_error>
#include <tuple>
#include <vector>
#include <type_traits>
#include <string>
#include <thread>
#include <new>
#include <fstream>
