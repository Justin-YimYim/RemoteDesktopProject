#pragma once

#ifndef TCPCLIENT
#define TCPCLIENT

#include "pch.hpp"
#include "NonCopyable.hpp"
#include "TCPSocket.hpp"

namespace RemoteDesk {
	class TCPClient : public nonCopyable<TCPClient>{
	public:
		inline explicit TCPClient(sockaddr_in& connectEndpoint) try : mSocket{} {
			mSocket.Connect(connectEndpoint);
		}catch (...) {
			throw;
		}

		inline explicit TCPClient(sockaddr_in& connectEndpoint, std::nothrow_t) noexcept : mSocket{std::nothrow} {
			if (!mSocket.Connect(connectEndpoint, std::nothrow)) {

			}
		}

		inline explicit TCPClient(TCPSocket socket) noexcept : mSocket{std::move(socket)} {}

		inline ~TCPClient() noexcept {}

	private:
		TCPSocket mSocket{};

	};

}

#endif