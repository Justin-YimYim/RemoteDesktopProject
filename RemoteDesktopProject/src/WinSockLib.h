#pragma once
#include "pch.h"

#ifndef WINSOCK_LIB
#define WINSOCK_LIB

namespace RemoteDesk {

	class WinsockLib final{
		private:
			bool isInit;
			WSADATA mWsaData;

		public:
			explicit WinsockLib();
			WinsockLib(const WinsockLib& T) = delete;
			WinsockLib(WinsockLib&& T) noexcept;
			WinsockLib& operator=(const WinsockLib& T) = delete;
			WinsockLib& operator=(WinsockLib&& T) noexcept;
			~WinsockLib() noexcept;
			const WSADATA& getWSAData() noexcept;
	};
}

#endif