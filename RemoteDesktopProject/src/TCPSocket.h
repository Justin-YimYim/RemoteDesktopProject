#pragma once

#include "pch.h"
#include "SafeSocket.h"

#ifndef TCPSOCKET
#define TCPSOCKET

namespace RemoteDesk {

	class TCPSocket
	{

		public: 
			TCPSocket();
			TCPSocket(const TCPSocket&) = delete;
			TCPSocket& operator=(const TCPSocket&) = delete;
			TCPSocket(TCPSocket&&) = default;
			TCPSocket& operator=(TCPSocket&&) = default;	

			~TCPSocket() noexcept;
			void ShutDown();
			void Bind(const sockaddr_in& address);
			void Connect(const sockaddr_in& address);
			int Send(const std::string& data, const int flag = 0);
			std::string Receive(const int& numberOfBytes, const int flag = 0);
			TCPSocket Accept();
			void Listen(const int backlog = 1);
		private: 
			SafeSocket mSocket{};
			explicit TCPSocket(SOCKET socket) noexcept : mSocket(socket) {}
	
	};

}
#endif
