#pragma once
#include "Track.h"
#include "Album.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT FullTrack : public Track
				{
				public:
					FullTrack();
					virtual ~FullTrack();

					const Album& getAlbum() const;
					const std::map<std::string, std::string>& getExternalIds() const;
					int getPopularity() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					Album _album;
					std::map<std::string, std::string> _external_ids;
					int _popularity;
				};
			}
		}
	}
}
