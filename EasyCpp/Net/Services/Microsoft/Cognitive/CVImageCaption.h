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
					class DLL_EXPORT CVImageCaption : public Serialize::Serializable
					{
					public:
						CVImageCaption();
						virtual ~CVImageCaption();

						const std::string& getText() const;
						double getConfidence() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _text;
						double _confidence;
					};
				}
			}
		}
	}
}
