#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>
#include <vector>
#include <map>
#include "Image.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Album : public Serialize::Serializable
				{
				public:
					Album();
					virtual ~Album();

					const std::string& getAlbumType();
					const std::vector<std::string>& getAvailableMarkets();
					const std::map<std::string, std::string>& getExternalUrls();
					const std::string& getHref();
					const std::string& getID();
					const std::vector<Image>& getImages();
					const std::string& getName();
					const std::string& getURI();

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _album_type;
					std::vector<std::string> _available_markets;
					std::map<std::string, std::string> _external_urls;
					std::string _href;
					std::string _id;
					std::vector<Image> _images;
					std::string _name;
					std::string _uri;
				};

			}
		}
	}
}
