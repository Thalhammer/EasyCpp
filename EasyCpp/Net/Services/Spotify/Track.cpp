#include "Track.h"
#include <cstddef>
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
				Track::~Track()
				{}

				const std::vector<Artist>& Track::getArtists() const
				{
					return _artists;
				}

				const std::vector<std::string>& Track::getAvailableMarkets() const
				{
					return _available_markets;
				}

				int Track::getDiscNumber() const
				{
					return _disc_number;
				}

				int Track::getDurationMs() const
				{
					return _duration_ms;
				}

				bool Track::getExplicit() const
				{
					return _explicit;
				}

				const std::map<std::string, std::string>& Track::getExternalUrls() const
				{
					return _external_urls;
				}

				const std::string & Track::getHref() const
				{
					return _href;
				}

				const std::string & Track::getID() const
				{
					return _id;
				}

				const Nullable<bool>& Track::getIsPlayable() const
				{
					return _is_playable;
				}

				const Nullable<TrackLink>& Track::getLinkedFrom() const
				{
					return _linked_from;
				}

				const std::string & Track::getName() const
				{
					return _name;
				}

				const std::string & Track::getPreviewUrl() const
				{
					return _preview_url;
				}

				int Track::getTrackNumber() const
				{
					return _track_number;
				}

				const std::string & Track::getURI() const
				{
					return _uri;
				}

				AnyValue Track::toAnyValue() const
				{
					return Bundle({
						{ "artists", toAnyArraySerialize(_artists) },
						{ "available_markets", toAnyArray(_available_markets) },
						{ "disc_number", _disc_number },
						{ "duration_ms", _duration_ms },
						{ "explicit", _explicit },
						{ "external_urls", Bundle(_external_urls) },
						{ "href", _href },
						{ "id", _id },
						{ "is_playable", _is_playable.IsNull() ? AnyValue(nullptr) : AnyValue(_is_playable.Value()) },
						{ "linked_from", _linked_from.IsNull() ? AnyValue(nullptr) : AnyValue(_linked_from->toAnyValue()) },
						{ "name", _name },
						{ "preview_url", _preview_url },
						{ "track_number", _track_number },
						{ "uri", _uri }
					});
				}

				void Track::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_artists = fromAnyArray<Artist>(b.get<AnyArray>("artists"));
					if (!b.get("available_markets").isType<nullptr_t>())
						_available_markets = fromAnyArray<std::string>(b.get<AnyArray>("available_markets"));
					_disc_number = b.get<int>("disc_number");
					_duration_ms = b.get<int>("duration_ms");
					_explicit = b.get<bool>("explicit");
					_external_urls = b.get<Bundle>("external_urls").getMap<std::string>();
					_href = b.get<std::string>("href");
					_id = b.get<std::string>("id");
					if (b.isSet("is_playable"))
						_is_playable = b.get<bool>("is_playable");
					if (b.isSet("linked_from"))
						_linked_from = b.get<TrackLink>("linked_from");
					_name = b.get<std::string>("name");
					_preview_url = b.get("preview_url").isType<nullptr_t>() ? "" : b.get<std::string>("preview_url");
					_track_number = b.get<int>("track_number");
					_uri = b.get<std::string>("uri");
				}

				Track::Track()
				{}
			}
		}
	}
}
