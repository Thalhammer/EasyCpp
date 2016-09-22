#include "Playlist.h"
#include "../../../Bundle.h"
#include "../../../AnyArray.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Playlist::Playlist()
				{
				}

				Playlist::~Playlist()
				{
				}

				bool Playlist::getCollaborative() const
				{
					return _collaborative;
				}

				const std::map<std::string, std::string>& Playlist::getExternalUrls() const
				{
					return _external_urls;
				}

				const std::string & Playlist::getHref() const
				{
					return _href;
				}

				const std::string & Playlist::getID() const
				{
					return _id;
				}

				const std::vector<Image>& Playlist::getImages() const
				{
					return _images;
				}

				const std::string & Playlist::getName() const
				{
					return _name;
				}

				const PublicUser & Playlist::getOwner() const
				{
					return _owner;
				}

				bool Playlist::getPublic() const
				{
					return _public;
				}

				const std::string & Playlist::getSnapshotID() const
				{
					return _snapshot_id;
				}

				size_t Playlist::getNumTracks() const
				{
					return _num_tracks;
				}

				const std::string & Playlist::getURI() const
				{
					return _uri;
				}

				AnyValue Playlist::toAnyValue() const
				{
					return Bundle({
						{"collaborative", _collaborative},
						{"external_urls", Bundle(_external_urls)},
						{"href", _href},
						{"id", _id},
						{"images", toAnyArraySerialize(_images)},
						{"name", _name},
						{"owner", _owner.toAnyValue()},
						{"public", _public},
						{"snapshot_id", _snapshot_id},
						{"tracks", Bundle({
							{"href", "https://api.spotify.com/v1/users/" + _owner.getID() + "/playlists/" + _id + "/tracks"},
							{"total", _num_tracks}
						})},
						{"uri", _uri}
					});
				}

				void Playlist::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_collaborative = b.get<bool>("collaborative");
					_external_urls = b.get<Bundle>("external_urls").getMap<std::string>();
					_href = b.get<std::string>("href");
					_id = b.get<std::string>("id");
					_images = fromAnyArray<Image>(b.get<AnyArray>("images"));
					_name = b.get<std::string>("name");
					_owner = b.get<PublicUser>("owner");
					_public = b.get("public").isType<nullptr_t>() || b.get<bool>("public");
					_snapshot_id = b.get<std::string>("snapshot_id");
					_num_tracks = b.get<Bundle>("tracks").get<size_t>("total");
					_uri = b.get<std::string>("uri");
				}
			}
		}
	}
}