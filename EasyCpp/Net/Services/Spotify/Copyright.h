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
				class DLL_EXPORT Copyright : public Serialize::Serializable
				{
				public:
					Copyright();
					virtual ~Copyright();

					const std::string& getText() const;
					const std::string& getType() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _text;
					std::string _type;
				};
			}
		}
	}
}
