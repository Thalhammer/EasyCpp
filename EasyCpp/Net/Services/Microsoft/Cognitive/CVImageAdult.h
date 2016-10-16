#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"

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
					class DLL_EXPORT CVImageAdult : public Serialize::Serializable
					{
					public:
						CVImageAdult();
						virtual ~CVImageAdult();

						bool isAdultContent() const;
						bool isRacyContent() const;
						double adultScore() const;
						double racyScore() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						bool _adult_content;
						bool _racy_content;
						double _adult_score;
						double _racy_score;
					};
				}
			}
		}
	}
}
