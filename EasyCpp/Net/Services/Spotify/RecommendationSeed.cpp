#include "RecommendationSeed.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				RecommendationSeed::RecommendationSeed()
				{
				}

				RecommendationSeed::~RecommendationSeed()
				{
				}

				const std::string & RecommendationSeed::getHref() const
				{
					return _href;
				}

				const std::string & RecommendationSeed::getID() const
				{
					return _id;
				}

				const std::string & RecommendationSeed::getType() const
				{
					return _type;
				}

				size_t RecommendationSeed::getInitialPoolSize() const
				{
					return _initialPoolSize;
				}

				size_t RecommendationSeed::getAfterFilteringSize() const
				{
					return _afterFilteringSize;
				}

				size_t RecommendationSeed::getAfterRelinkingSize() const
				{
					return _afterRelinkingSize;
				}

				AnyValue RecommendationSeed::toAnyValue() const
				{
					return Bundle({
						{ "afterRelinkingSize", _afterRelinkingSize },
						{ "afterFilteringSize", _afterFilteringSize },
						{ "href", _href },
						{ "initialPoolSize", _initialPoolSize },
						{ "id", _id },
						{ "type", _type }
					});
				}

				void RecommendationSeed::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_afterFilteringSize = b.get<size_t>("afterFilteringSize");
					_afterRelinkingSize = b.get<size_t>("afterRelinkingSize");
					_href = b.get<std::string>("href");
					_initialPoolSize = b.get<size_t>("initialPoolSize");
					_id = b.get<std::string>("id");
					_type = b.get<std::string>("type");
				}
			}
		}
	}
}