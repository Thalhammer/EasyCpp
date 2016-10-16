#pragma once
#include "CVDSMModel.h"

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
					class DLL_EXPORT CVDSMModelResult : public Serialize::Serializable
					{
					public:
						CVDSMModelResult();
						virtual ~CVDSMModelResult();

						const std::vector<CVDSMModel>& getModels() const;
						const std::string& getRequestId() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _request_id;
						std::vector<CVDSMModel> _models;
					};
				}
			}
		}
	}
}
