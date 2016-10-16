#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"
#include <string>
#include <vector>

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
					class DLL_EXPORT CVDSMModel : public Serialize::Serializable
					{
					public:
						CVDSMModel();
						virtual ~CVDSMModel();

						const std::string& getName() const;
						const std::vector<std::string>& getCategories() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _name;
						std::vector<std::string> _categories;
					};
				}
			}
		}
	}
}
