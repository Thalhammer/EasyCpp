#include "CVDSMModelResult.h"
#include "../../../../Bundle.h"
#include "../../../../AnyArray.h"

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
					CVDSMModelResult::CVDSMModelResult()
					{
					}

					CVDSMModelResult::~CVDSMModelResult()
					{
					}

					const std::vector<CVDSMModel>& CVDSMModelResult::getModels() const
					{
						return _models;
					}

					const std::string & CVDSMModelResult::getRequestId() const
					{
						return _request_id;
					}

					AnyValue CVDSMModelResult::toAnyValue() const
					{
						return Bundle({
							{ "models", toAnyArraySerialize(_models) },
							{ "requestId", _request_id }
						});
					}

					void CVDSMModelResult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_request_id = b.get<std::string>("requestId");
						_models = fromAnyArray<CVDSMModel>(b.get<AnyArray>("models"));
					}
				}
			}
		}
	}
}