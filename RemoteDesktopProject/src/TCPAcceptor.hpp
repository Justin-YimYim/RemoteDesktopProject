#pragma once

#ifndef TCPACCEPTOR
#define TCPACCEPTOR

#include "pch.hpp"
#include "NonCopyable.hpp"
#include "TCPSocket.hpp"

namespace RemoteDesk {
	class TCPAcceptor : public nonCopyable<TCPAcceptor> {
	public:
		inline explicit TCPAcceptor(const sockaddr_in& bindAddress, const int backlog) try : mSocket{} {
			mSocket.Bind(bindAddress);
			mSocket.Listen(backlog);
		} catch (...) {
			throw;
		}

		inline explicit TCPAcceptor(const sockaddr_in& bindAddress, const int backlog, std::nothrow_t) : mSocket{std::nothrow} {
			if (!mSocket) return;
			if (!mSocket.Bind(bindAddress, std::nothrow) || !mSocket.Listen(std::nothrow, backlog)) mSocket.Close();
		}

		inline TCPAcceptor(TCPAcceptor&&) noexcept = default;
		inline TCPAcceptor& operator=(TCPAcceptor&&) noexcept = default;
		
		~TCPAcceptor() noexcept {}

		inline bool IsValid() const noexcept {
			return mSocket.IsValid();
		}

		inline explicit operator bool() const noexcept {
			return mSocket.IsValid();
		}

		inline TCPSocket Accept() {
			return mSocket.Accept();
		}

		inline TCPSocket Accept(std::nothrow_t) noexcept {
			return mSocket.Accept(std::nothrow);
		}

	private:
		TCPSocket mSocket{};
	};

}

#endif