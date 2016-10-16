#include "CVImageAdult.h"
#include "../../../../Bundle.h"

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
					CVImageAdult::CVImageAdult()
					{
					}

					CVImageAdult::~CVImageAdult()
					{
					}

					bool CVImageAdult::isAdultContent() const
					{
						return _adult_content;
					}

					bool CVImageAdult::isRacyContent() const
					{
						return _racy_content;
					}

					double CVImageAdult::adultScore() const
					{
						return _adult_score;
					}

					double CVImageAdult::racyScore() const
					{
						return _racy_score;
					}

					AnyValue CVImageAdult::toAnyValue() const
					{
						return Bundle({
							{ "adultScore", _adult_score },
							{ "racyScore", _racy_score },
							{ "isAdultContent", _adult_content },
							{ "isRacyContent", _racy_content }
						});
					}

					void CVImageAdult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_adult_score = b.get<double>("adultScore");
						_racy_score = b.get<double>("racyScore");
						_adult_content = b.get<bool>("isAdultContent");
						_racy_content = b.get<bool>("isRacyContent");
					}
				}
			}
		}
	}
}