#pragma once

#ifndef NONCOPYABLE
#define NONCOPYABLE

namespace RemoteDesk {

	template <typename T>
	class nonCopyable {
	public:
		inline nonCopyable() = default;
		inline nonCopyable(nonCopyable&&) = default;
		inline nonCopyable& operator=(nonCopyable&&) = default;
		inline ~nonCopyable() = default;
		inline nonCopyable(const nonCopyable&) = delete;
		inline nonCopyable& operator=(const nonCopyable&) = delete;
	};
}

#endif
