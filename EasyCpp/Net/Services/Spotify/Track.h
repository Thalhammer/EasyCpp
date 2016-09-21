#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include "../../../Nullable.h"
#include "TrackLink.h"
#include <vector>
#include <memory>
#include "Artist.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Track : public Serialize::Serializable
				{
				public:
					Track();
					virtual ~Track();

					const std::vector<Artist>& getArtists() const;
					const std::vector<std::string>& getAvailableMarkets() const;
					int getDiscNumber() const;
					int getDurationMs() const;
					bool getExplicit() const;
					const std::map<std::string, std::string>& getExternalUrls() const;
					const std::string& getHref() const;
					const std::string& getID() const;
					const Nullable<bool>& getIsPlayable() const;
					const Nullable<TrackLink>& getLinkedFrom() const;
					const std::string& getName() const;
					const std::string& getPreviewUrl() const;
					int getTrackNumber() const;
					const std::string& getURI() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::vector<Artist> _artists;
					std::vector<std::string> _available_markets;
					int _disc_number;
					int _duration_ms;
					bool _explicit;
					std::map<std::string, std::string> _external_urls;
					std::string _href;
					std::string _id;
					Nullable<bool> _is_playable;
					Nullable<TrackLink> _linked_from;
					std::string _name;
					std::string _preview_url;
					int _track_number;
					std::string _uri;
				};
			}
		}
	}
}
