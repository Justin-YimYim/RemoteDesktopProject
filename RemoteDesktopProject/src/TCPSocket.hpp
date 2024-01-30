#pragma once

#ifndef TCPSOCKET
#define TCPSOCKET

#include "pch.hpp"
#include "NonCopyable.hpp"
#include "SafeSocket.hpp"

namespace RemoteDesk {

	class TCPSocket : public nonCopyable<TCPSocket>
	{

		public: 
			 TCPSocket();
			 TCPSocket(std::nothrow_t) noexcept;
			
			 TCPSocket(TCPSocket&&) = default;
			 TCPSocket& operator=(TCPSocket&&) = default;	
			
			 ~TCPSocket() noexcept;
			
			 bool ShutDown() noexcept;
			
			 void Bind(const sockaddr_in& address);
			 bool Bind(const sockaddr_in& address, std::nothrow_t) noexcept;
			
			 void Connect(const sockaddr_in& address);
			 bool Connect(const sockaddr_in& address, std::nothrow_t) noexcept;
			
			 int Send(const std::string& data, const int flag = 0);
			 int Send(const void* buffer, const int length, std::nothrow_t, const int flag = 0) noexcept;
			
			 std::string Receive(const int& numberOfBytes, const int flag = 0);
			 int Receive(void* buffer, const int length, std::nothrow_t, const int flag = 0) noexcept;
			
			 TCPSocket Accept();
			 TCPSocket Accept(std::nothrow_t) noexcept;
			
			 void Listen(const int backlog = 1);
			 bool Listen(std::nothrow_t, const int backlog = 1) noexcept;
			
			 sockaddr_in GetSocketAddress() const noexcept;
			 sockaddr_in GetPeerSocketAddress() const noexcept;
		
			 void Close() noexcept;
			
			 bool IsValid() const noexcept {
				return mSocket.IsValid();
			}
			
			 operator bool() const noexcept {
				return mSocket.IsValid();
			}

		private: 
			SafeSocket mSocket{};
			 explicit TCPSocket(SOCKET socket) noexcept : mSocket(socket) {}
	
	};

}
#endif
