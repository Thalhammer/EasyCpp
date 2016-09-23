#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include "Playlist.h"
#include "Paging.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT FeaturedPlaylists : public Serialize::Serializable
				{
				public:
					FeaturedPlaylists();
					virtual ~FeaturedPlaylists();

					const std::string& getMessage() const;
					const Paging<Playlist>& getPlaylists() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _message;
					Paging<Playlist> _playlists;
				};
			}
		}
	}
}
