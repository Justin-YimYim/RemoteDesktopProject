#include "TCPSocket.hpp"

namespace RemoteDesk {
	TCPSocket::TCPSocket() {
		mSocket.Reset(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
		if (!mSocket) {
			throw std::system_error(WSAGetLastError(), std::system_category());
		}
	}

	TCPSocket::TCPSocket(std::nothrow_t) noexcept {
		mSocket.Reset(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	}


	TCPSocket::~TCPSocket() noexcept{
		ShutDown();
	}

	bool TCPSocket::ShutDown() noexcept {
		return shutdown(mSocket.GetResource(), SD_BOTH) != SOCKET_ERROR;
	}

	void TCPSocket::Bind(const sockaddr_in& address) {
		const int bindResult = bind(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in));
		if (bindResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}

	bool TCPSocket::Bind(const sockaddr_in& address, std::nothrow_t) noexcept {
		return bind(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in)) != SOCKET_ERROR;
	}

	void TCPSocket::Connect(const sockaddr_in& address) {
		const int connectResult = connect(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in));
		if (connectResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}

	bool TCPSocket::Connect(const sockaddr_in& address, std::nothrow_t) noexcept {
		return connect(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in)) != SOCKET_ERROR;
	}

	int TCPSocket::Send(const std::string& data, const int flag) {
		if (data.empty()) throw std::invalid_argument("Data is Empty");
		if (data.size() > size_t{ std::numeric_limits<int>::max() }) throw std::invalid_argument("Data Overflow");

		const int sendResult = send(mSocket.GetResource(), data.data(), int(data.size()), flag);
		if (sendResult == SOCKET_ERROR) return 0;
		return sendResult;
	}

	int TCPSocket::Send(const void* buffer, const int length, std::nothrow_t, const int flag) noexcept {
		return send(mSocket.GetResource(), reinterpret_cast<const char*>(buffer), length, flag);
	}

	std::string TCPSocket::Receive(const int& numberOfBytes, const int flag) {
		if (numberOfBytes == 0) throw std::invalid_argument("Bytes should be larger than zero");

		std::string buffer(numberOfBytes, ' ');
		const int receiveResult = recv(mSocket.GetResource(), buffer.data(), numberOfBytes, 0);

		if (receiveResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
		if (receiveResult == 0) buffer.clear();

		return buffer;
	}

	int TCPSocket::Receive(void* buffer, const int length, std::nothrow_t, const int flag) noexcept {
		return recv(mSocket.GetResource(), reinterpret_cast<char*>(buffer), length, flag);
	}

	TCPSocket TCPSocket::Accept() {
		TCPSocket socket{ accept(mSocket.GetResource(), nullptr, nullptr) };

		if (!socket.mSocket) throw std::system_error(WSAGetLastError(), std::system_category());
		
		return socket;
	}


	TCPSocket TCPSocket::Accept(std::nothrow_t) noexcept {
		return TCPSocket{ accept(mSocket.GetResource(), nullptr, nullptr) };
	}


	void TCPSocket::Listen(const int backlog) {
		const int listenResult = listen(mSocket.GetResource(), backlog);
		if (listenResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}

	bool TCPSocket::Listen(std::nothrow_t, const int backlog) noexcept {
		return listen(mSocket.GetResource(), backlog) != SOCKET_ERROR;
	}

	void TCPSocket::Close() noexcept {
		// Not Yet Started Listen
		// Called when constructor fails
		mSocket.Reset();
	}


	sockaddr_in TCPSocket::GetSocketAddress() const noexcept {
		sockaddr_in address{};
		int length = sizeof(address);
		std::ignore = getsockname(mSocket.GetResource(), reinterpret_cast<sockaddr*> (&address), &length);
		return address;
	}

	sockaddr_in TCPSocket::GetPeerSocketAddress() const noexcept {
		sockaddr_in address{};
		int length = sizeof(address);
		std::ignore = getpeername(mSocket.GetResource(), reinterpret_cast<sockaddr*> (&address), &length);
		return address;
	}
}