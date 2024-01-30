#pragma once

#ifndef TCPSTREAM
#define TCPSTREAM

#include "pch.hpp"
#include "NonCopyable.hpp"
#include "TCPSocket.hpp"

namespace RemoteDesk {
	class TCPStream : public nonCopyable<TCPStream>{
	public:
		static constexpr int C_Buffersize = 1024 * 8;

		inline explicit TCPStream(TCPSocket& socket) : mSocket{ socket } {
			if (!mSocket) {
				throw std::runtime_error("Invalid Socket");
			}
		}

		inline explicit TCPStream(TCPSocket& socket, std::nothrow_t): mSocket{ socket }{}

		inline TCPStream(TCPStream &&) noexcept = default;
		inline TCPStream& operator=(TCPStream&&) noexcept = default;

		inline ~TCPStream() noexcept {}

		inline const TCPSocket& GetTCPSocket() const noexcept {
			return mSocket;
		}

		template<typename T>
		requires(std::is_integral_v<T>)
		inline void Send(const T value, const int flags = 0) {
			const auto buffer{ std::bit_cast<std::array<std::byte, sizeof(T)> > (value)};
			const int result = mSocket.Send(buffer.data(), sizeof(T), std::nothrow, sizeof(T));

			if (result == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category);
			if (result != sizeof(T)) throw std::runtime_error("Failed to send all bytes");
		}

		template<typename T>
		requires(std::is_integral_v<T>)
		inline T Receive(const int flags = 0) {
			std::array<std::byte, sizeof(T)> buffer{};
			const int result = mSocket.Receive(buffer.data(), sizeof(T), std::nothrow, flags);
		
			if (result == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category);
			if (result != sizeof(T)) throw std::runtime_error("Failed to receive all bytes");
			if (result == 0) throw std::runtime_error("Sockets has been disconnected");
			
			return std::bit_cast<T> (buffer);
		}

		template<typename T>
		requires(std::is_integral_v<T>)
		inline bool Send(const T value, std::nothrow_t, const int flags = 0) noexcept {
			const auto buffer{ std::bit_cast<std::array<std::byte, sizeof(T)>> (value) };
			const int result = mSocket.Send(buffer.data(), sizeof(T), std::nothrow, sizeof(T));

			if (result == SOCKET_ERROR || result != sizeof(T)) return false;
			return true;
		}

		template<typename T>
		requires(std::is_integral_v<T>)
		inline bool Receive(T& value, std::nothrow_t, const int flags = 0) noexcept {
			std::array<std::byte, sizeof(T)> buffer{};
			const int result = mSocket.Receive(buffer.data(), sizeof(T), std::nothrow, flags);

			if (result == SOCKET_ERROR || result != sizeof(T) || result == 0) return false;
			value = std::bit_cast<T>(buffer);
			return true;
		}

		inline bool IsValid() const noexcept {
			return mSocket.IsValid();
		}

		inline explicit operator bool() const noexcept {
			return mSocket.IsValid();
		}

		inline void Send(std::vector<std::byte> &bytes, const int flag = 0){
			std::intptr_t sentSize{ 0 };
			while (sentSize != static_cast<intptr_t> (bytes.size())) {
				const auto it{bytes.begin() + sentSize};
				const auto distanceToEnd{ std::distance(it, bytes.end()) };
				const int result = mSocket.Send(&(*it), static_cast<int> (std::min(C_Buffersize, distanceToEnd)), std::nothrow);

				if (result == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
				if (result == 0) throw std::runtime_error("Sockets has been disconnected");

				sentSize += static_cast<intptr_t>(result);
			}
		}

		inline void Receive(std::vector<std::byte>& bytes, const int flag = 0) {
			if (bytes.empty()) throw std::invalid_argument("Bytes is Empty!");
			std::intptr_t sentSize{ 0 };
			while (sentSize != static_cast<intptr_t> (bytes.size())) {
				const auto it{ bytes.begin() + sentSize };
				const auto distanceToEnd{ std::distance(it, bytes.end()) };
				const int result = mSocket.Receive(&(*it), static_cast<int> (std::min(C_Buffersize, distanceToEnd)), std::nothrow);

				if (result == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
				if (result == 0) throw std::runtime_error("Sockets has been disconnected");

				sentSize += static_cast<intptr_t>(result);
			}
		}

	private: 
		TCPSocket& mSocket;
	};
}

#endif