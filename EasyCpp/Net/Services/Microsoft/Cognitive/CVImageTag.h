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
					class DLL_EXPORT CVImageTag : public Serialize::Serializable
					{
					public:
						CVImageTag();
						virtual ~CVImageTag();

						const std::string& getName() const;
						double getConfidence() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _name;
						double _confidence;
					};
				}
			}
		}
	}
}
