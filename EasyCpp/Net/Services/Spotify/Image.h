#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Image : public Serialize::Serializable
				{
				public:
					Image();
					virtual ~Image();
					const std::string& getURL() const;
					uint64_t getHeight() const;
					uint64_t getWidth() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _url;
					uint64_t _height;
					uint64_t _width;
				};
			}
		}
	}
}
