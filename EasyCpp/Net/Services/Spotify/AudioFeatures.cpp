#include "AudioFeatures.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				AudioFeatures::~AudioFeatures()
				{
				}
				float AudioFeatures::getAcousticness() const
				{
					return _acousticness;
				}
				const std::string & AudioFeatures::getAnalysisUrl() const
				{
					return _analysis_url;
				}
				float AudioFeatures::getDanceAbility() const
				{
					return _danceability;
				}
				int AudioFeatures::getDurationMs() const
				{
					return _duration_ms;
				}
				float AudioFeatures::getEnergy() const
				{
					return _energy;
				}
				const std::string & AudioFeatures::getID() const
				{
					return _id;
				}
				float AudioFeatures::getInstrumentalness() const
				{
					return _instrumentalness;
				}
				int AudioFeatures::getKey() const
				{
					return _key;
				}
				float AudioFeatures::getLiveness() const
				{
					return _liveness;
				}
				float AudioFeatures::getLoudness() const
				{
					return _loudness;
				}
				int AudioFeatures::getMode() const
				{
					return _mode;
				}
				float AudioFeatures::getSpeechiness() const
				{
					return _speechiness;
				}
				float AudioFeatures::getTempo() const
				{
					return _tempo;
				}
				const std::string & AudioFeatures::getTrackHref() const
				{
					return _track_href;
				}
				const std::string & AudioFeatures::getURI() const
				{
					return _uri;
				}
				float AudioFeatures::getValence() const
				{
					return _valence;
				}
				AnyValue AudioFeatures::toAnyValue() const
				{
					return AnyValue();
				}
				void AudioFeatures::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_acousticness = b.get<float>("acousticness");
					_analysis_url = b.get<std::string>("analysis_url");
					_danceability = b.get<float>("danceability");
					_duration_ms = b.get<int>("duration_ms");
					_energy = b.get<float>("energy");
					_id = b.get<std::string>("id");
					_instrumentalness = b.get<float>("instrumentalness");
					_key = b.get<int>("key");
					_liveness = b.get<float>("liveness");
					_loudness = b.get<float>("loudness");
					_mode = b.get<int>("mode");
					_speechiness = b.get<float>("speechiness");
					_tempo = b.get<float>("tempo");
					_time_signature = b.get<int>("time_signature");
					_track_href = b.get<std::string>("track_href");
					_uri = b.get<std::string>("uri");
					_valence = b.get<float>("valence");
				}
				AudioFeatures::AudioFeatures()
				{
				}
			}
		}
	}
}