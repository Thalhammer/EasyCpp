#include "CVDSMModel.h"
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
					CVDSMModel::CVDSMModel()
					{
					}

					CVDSMModel::~CVDSMModel()
					{
					}

					const std::string & CVDSMModel::getName() const
					{
						return _name;
					}

					const std::vector<std::string>& CVDSMModel::getCategories() const
					{
						return _categories;
					}

					AnyValue CVDSMModel::toAnyValue() const
					{
						return Bundle({
							{"name", _name},
							{"categories", toAnyArray(_categories)}
						});
					}

					void CVDSMModel::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_name = b.get<std::string>("name");
						_categories = fromAnyArray<std::string>(b.get<AnyArray>("categories"));
					}
				}
			}
		}
	}
}