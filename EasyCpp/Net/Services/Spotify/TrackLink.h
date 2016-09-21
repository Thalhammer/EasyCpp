#pragma once
#include <map>
#include <string>
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT TrackLink : public Serialize::Serializable
				{
				public:
					TrackLink();
					virtual ~TrackLink();

					const std::map<std::string, std::string>& getExternalUrls() const;
					const std::string& getHref() const;
					const std::string& getID() const;
					const std::string& getURI() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::map<std::string, std::string> _external_urls;
					std::string _href;
					std::string _id;
					std::string _uri;
				};
			}
		}
	}
}
