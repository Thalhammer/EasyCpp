#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Microsoft
			{
				namespace Cognitive
				{
					class DLL_EXPORT CVImageMetadata : public Serialize::Serializable
					{
					public:
						CVImageMetadata();
						virtual ~CVImageMetadata();

						const std::string& getFormat() const;
						size_t getWidth() const;
						size_t getHeight() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _format;
						size_t _width;
						size_t _height;
					};
				}
			}
		}
	}
}
