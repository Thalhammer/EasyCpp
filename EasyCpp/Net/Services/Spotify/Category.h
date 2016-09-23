#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include "Image.h"
#include <string>
#include <vector>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Category : public Serialize::Serializable
				{
				public:
					Category();
					virtual ~Category();

					const std::string& getHref() const;
					const std::string& getID() const;
					const std::string& getName() const;
					const std::vector<Image>& getIcons() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _href;
					std::string _id;
					std::string _name;
					std::vector<Image> _icons;
				};
			}
		}
	}
}
