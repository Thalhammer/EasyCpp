#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include "RecommendationSeed.h"
#include "Track.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT RecommendationResponse : public Serialize::Serializable
				{
				public:
					RecommendationResponse();
					virtual ~RecommendationResponse();

					const std::vector<Track>& getTracks() const;
					const std::vector<RecommendationSeed>& getSeeds() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::vector<Track> _tracks;
					std::vector<RecommendationSeed> _seeds;
				};
			}
		}
	}
}
