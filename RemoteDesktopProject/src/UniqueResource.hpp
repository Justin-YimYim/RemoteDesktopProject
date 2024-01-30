#pragma once

#include "pch.hpp"

#ifndef UNIQUE_RESOURCE
#define UNIQUE_RESOURCE

namespace RemoteDesk {

	// RAII
	template<typename ResourceType, typename Deleter, ResourceType InvalidResourceValue = nullptr>
	class UniqueResource {
		private:
			ResourceType mResource{ InvalidResourceValue };
		public:
			UniqueResource() = default;
			
			explicit UniqueResource(const ResourceType res) noexcept : mResource{ res } {}

			UniqueResource(const UniqueResource&) = delete;
			UniqueResource& operator=(const UniqueResource&) = delete;

			UniqueResource(UniqueResource&& rhs) noexcept : mResource{std::exchange( rhs.mResource, InvalidResourceValue)} {}
			UniqueResource& operator=(UniqueResource&& rhs) noexcept {
				mResource = std::exchange(rhs.mResource, InvalidResourceValue);
				return *this;
			}

			~UniqueResource() noexcept {
				this->Reset();
			}

			inline explicit operator bool() const noexcept {
				return this->IsValid();
			}
	
			bool IsValid() const noexcept {
				return mResource != InvalidResourceValue; 
			}

			void Reset(ResourceType resource = InvalidResourceValue) noexcept {
				if (this->IsValid()) {
					Deleter deleter{};
					deleter(mResource);
				}
				mResource = resource;
			}

			[[nodiscard]]
			ResourceType ReleaseOwnership() noexcept {
				return std::exchange(mResource, InvalidResourceValue);
			}

			ResourceType GetResource() const noexcept {
				return mResource;
			}
	};
}

#endif