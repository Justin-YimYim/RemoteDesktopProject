#pragma once

#ifndef TCPRESOLVER
#define TCPRESOLVER 

#include "pch.hpp"
#include "NonCopyable.hpp"

namespace RemoteDesk {

	class TCPResolver
	{
	public:
		class Result : public nonCopyable<Result> {
		public:
			~Result() noexcept {
				if (mAddressInfo) freeaddrinfo(mAddressInfo);
			}

			Result(Result&& results) noexcept : mAddressInfo(std::exchange(results.mAddressInfo, nullptr)) {}
			Result& operator=(Result&& rhs) noexcept {
				mAddressInfo = std::exchange(rhs.mAddressInfo, nullptr);
				return *this;
			}

			const addrinfo* const GetNativeAddrInfo() const noexcept {
				return mAddressInfo;
			}

			sockaddr_in GetEndpoint() const noexcept {
				return *reinterpret_cast<sockaddr_in*>(mAddressInfo->ai_addr);
			}

			inline bool HasEndpoint() const noexcept {
				return mAddressInfo != nullptr;
			}

			inline explicit operator bool() const noexcept{
				return mAddressInfo != nullptr;
			}

			private:
				addrinfo* mAddressInfo{nullptr};
				inline explicit Result(addrinfo* results) noexcept : mAddressInfo(results) {}
				
				friend class RemoteDesk::TCPResolver;
		};

		static Result Resolve(const std::string& host, const std::string& service) noexcept {
			const addrinfo hint{
				.ai_family = AF_INET,
				.ai_socktype = SOCK_STREAM,
				.ai_protocol = IPPROTO_TCP,
			};
			addrinfo* result{ nullptr };

			std::ignore = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
			return Result{ result };
		}

	};

}
#endif