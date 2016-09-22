#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include "Image.h"
#include "PublicUser.h"
#include "FullTrack.h"
#include <string>
#include <map>
#include <vector>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Playlist : public Serialize::Serializable
				{
				public:
					Playlist();
					virtual ~Playlist();
					
					bool getCollaborative() const;
					const std::map<std::string, std::string>& getExternalUrls() const;
					const std::string& getHref() const;
					const std::string& getID() const;
					const std::vector<Image>& getImages() const;
					const std::string& getName() const;
					const PublicUser& getOwner() const;
					bool getPublic() const;
					const std::string& getSnapshotID() const;
					size_t getNumTracks() const;
					const std::string& getURI() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					bool _collaborative;
					std::map<std::string, std::string> _external_urls;
					std::string _href;
					std::string _id;
					std::vector<Image> _images;
					std::string _name;
					PublicUser _owner;
					bool _public;
					std::string _snapshot_id;
					size_t _num_tracks;
					std::string _uri;
				};
			}
		}
	}
}
