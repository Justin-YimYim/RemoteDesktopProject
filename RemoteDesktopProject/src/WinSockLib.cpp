#include "WinSockLib.h"


RemoteDesk::WinsockLib::WinsockLib(){
	const int startupResult = WSAStartup(WINSOCK_VERSION, &mWsaData);
	if (startupResult) {
		throw std::system_error(startupResult, std::system_category());
	}
	isInit = true;
}

// move constructor
RemoteDesk::WinsockLib::WinsockLib(WinsockLib&& T) noexcept {
	isInit = std::exchange(T.isInit, 0);
	mWsaData = T.mWsaData;
}

// move assignment
RemoteDesk::WinsockLib& RemoteDesk::WinsockLib::operator=(WinsockLib&& T) noexcept{
	isInit = std::exchange(T.isInit, 0);
	mWsaData = T.mWsaData;
	return *this;
}

// destructor
RemoteDesk::WinsockLib::~WinsockLib() noexcept {
	if (isInit) std::ignore = WSACleanup();
}

const WSADATA& RemoteDesk::WinsockLib::getWSAData() noexcept {
	return mWsaData;
}
