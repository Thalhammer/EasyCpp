#include "RecommendationResponse.h"
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
				RecommendationResponse::RecommendationResponse()
				{
				}

				RecommendationResponse::~RecommendationResponse()
				{
				}

				const std::vector<Track>& RecommendationResponse::getTracks() const
				{
					return _tracks;
				}

				const std::vector<RecommendationSeed>& RecommendationResponse::getSeeds() const
				{
					return _seeds;
				}

				AnyValue RecommendationResponse::toAnyValue() const
				{
					return Bundle({
						{ "tracks", toAnyArraySerialize(_tracks) },
						{ "seeds", toAnyArraySerialize(_seeds) }
					});
				}

				void RecommendationResponse::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_tracks = fromAnyArray<Track>(b.get<AnyArray>("tracks"));
					_seeds = fromAnyArray<RecommendationSeed>(b.get<AnyArray>("seeds"));
				}
			}
		}
	}
}