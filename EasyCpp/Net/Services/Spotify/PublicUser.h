#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>
#include <map>
#include <vector>
#include "Followers.h"
#include "Image.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT PublicUser : public Serialize::Serializable
				{
				public:
					PublicUser();
					virtual ~PublicUser();
					const std::string& getDisplayName() const;
					const std::map<std::string, std::string>& getExternalUrls() const;
					const Followers& getFollowers() const;
					const std::string& getHref() const;
					const std::string& getID() const;
					const std::vector<Image>& getImages() const;
					const std::string& getURI() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _display_name;
					std::string _href;
					std::string _id;
					std::string _uri;
					std::map<std::string, std::string> _external_urls;
					Followers _followers;
					std::vector<Image> _images;
				};

			}
		}
	}
}
