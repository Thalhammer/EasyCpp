#pragma once
#include "CVImageFaceRectangle.h"
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
					class DLL_EXPORT CVImageFace : public Serialize::Serializable
					{
					public:
						CVImageFace();
						virtual ~CVImageFace();

						double getAge() const;
						const std::string& getGender() const;
						const CVImageFaceRectangle& getRectangle() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						double _age;
						std::string _gender;
						CVImageFaceRectangle _rectangle;
					};
				}
			}
		}
	}
}
