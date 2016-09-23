#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT RecommendationSeed : public Serialize::Serializable
				{
				public:
					RecommendationSeed();
					virtual ~RecommendationSeed();

					const std::string& getHref() const;
					const std::string& getID() const;
					const std::string& getType() const;
					size_t getInitialPoolSize() const;
					size_t getAfterFilteringSize() const;
					size_t getAfterRelinkingSize() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					size_t _initialPoolSize;
					size_t _afterFilteringSize;
					size_t _afterRelinkingSize;
					std::string _href;
					std::string _id;
					std::string _type;
				};
			}
		}
	}
}
