#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT AudioFeatures : public Serialize::Serializable
				{
				public:
					AudioFeatures();
					virtual ~AudioFeatures();

					float getAcousticness() const;
					const std::string& getAnalysisUrl() const;
					float getDanceAbility() const;
					int getDurationMs() const;
					float getEnergy() const;
					const std::string& getID() const;
					float getInstrumentalness() const;
					int getKey() const;
					float getLiveness() const;
					float getLoudness() const;
					int getMode() const;
					float getSpeechiness() const;
					float getTempo() const;
					const std::string& getTrackHref() const;
					const std::string& getURI() const;
					float getValence() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					float _acousticness;
					std::string _analysis_url;
					float _danceability;
					int _duration_ms;
					float _energy;
					std::string _id;
					float _instrumentalness;
					int _key;
					float _liveness;
					float _loudness;
					int _mode;
					float _speechiness;
					float _tempo;
					int _time_signature;
					std::string _track_href;
					std::string _uri;
					float _valence;
				};
			}
		}
	}
}
