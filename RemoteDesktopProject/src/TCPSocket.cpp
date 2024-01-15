#include "TCPSocket.h"

namespace RemoteDesk {
	TCPSocket::TCPSocket() {
		mSocket.Reset(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
		if (!mSocket) {
			throw std::system_error(WSAGetLastError(), std::system_category());
		}
	}

	TCPSocket::~TCPSocket() noexcept{
		this->ShutDown();
	}

	void TCPSocket::ShutDown() {
		std::ignore = shutdown(mSocket.GetResource(), SD_BOTH);
	}

	void TCPSocket::Bind(const sockaddr_in& address) {
		const int bindResult = bind(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in));
		if (bindResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}

	void TCPSocket::Connect(const sockaddr_in& address) {
		const int connectResult = connect(mSocket.GetResource(), reinterpret_cast<const sockaddr*> (&address), sizeof(sockaddr_in));
		if (connectResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}

	int TCPSocket::Send(const std::string& data, const int flag) {
		if (data.empty()) throw std::invalid_argument("Data is Empty");
		if (data.size() > size_t{ std::numeric_limits<int>::max() }) throw std::invalid_argument("Data Overflow");

		const int sendResult = send(mSocket.GetResource(), data.data(), int(data.size()), flag);
		if (sendResult == SOCKET_ERROR) return 0;
		return sendResult;
	}

	std::string TCPSocket::Receive(const int& numberOfBytes, const int flag) {
		if (numberOfBytes == 0) throw std::invalid_argument("Bytes should be larger than zero");

		std::string buffer(numberOfBytes, ' ');
		const int receiveResult = recv(mSocket.GetResource(), buffer.data(), numberOfBytes, 0);

		if (receiveResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
		if (receiveResult == 0) buffer.clear();

		return buffer;
	}

	TCPSocket TCPSocket::Accept() {
		TCPSocket socket{ accept(mSocket.GetResource(), nullptr, nullptr) };

		if (!socket.mSocket) throw std::system_error(WSAGetLastError(), std::system_category());
		
		return socket;
	}

	void TCPSocket::Listen(const int backlog) {
		const int listenResult = listen(mSocket.GetResource(), backlog);
		if (listenResult == SOCKET_ERROR) throw std::system_error(WSAGetLastError(), std::system_category());
	}
}