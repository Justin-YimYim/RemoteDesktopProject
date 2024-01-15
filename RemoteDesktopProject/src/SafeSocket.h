#pragma once

#include "pch.h"
#include "UniqueResource.h"

#ifndef SOCKET_DELETER
#define SOCKET_DELETER

namespace RemoteDesk {
	
	class SocketDeleter final
	{
	public:
		void operator()(SOCKET socket) {
			std::ignore = closesocket(socket);
		}
	};	

	using SafeSocket = UniqueResource<SOCKET, SocketDeleter, INVALID_SOCKET>;

}
#endif


