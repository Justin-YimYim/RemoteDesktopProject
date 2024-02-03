#pragma once

#ifndef SOCKET_DELETER
#define SOCKET_DELETER

#include "pch.hpp"
#include "UniqueResource.hpp"

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


