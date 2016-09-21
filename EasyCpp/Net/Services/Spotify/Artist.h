#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include <string>
#include <map>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Artist : public Serialize::Serializable
				{
				public:
					Artist();
					virtual ~Artist();

					const std::map<std::string, std::string>& getExternalUrls() const;
					const std::string& getHref() const;
					const std::string& getID() const;
					const std::string& getName() const;
					const std::string& getUri() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::map<std::string, std::string> _external_urls;
					std::string _href;
					std::string _id;
					std::string _name;
					std::string _uri;
				};
			}
		}
	}
}
