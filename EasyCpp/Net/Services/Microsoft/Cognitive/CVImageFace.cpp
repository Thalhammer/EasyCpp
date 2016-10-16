#include "CVImageFace.h"
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
					CVImageFace::CVImageFace()
					{
					}

					CVImageFace::~CVImageFace()
					{
					}

					double CVImageFace::getAge() const
					{
						return _age;
					}

					const std::string & CVImageFace::getGender() const
					{
						return _gender;
					}

					const CVImageFaceRectangle & CVImageFace::getRectangle() const
					{
						return _rectangle;
					}

					AnyValue CVImageFace::toAnyValue() const
					{
						return Bundle({
							{ "age", _age },
							{ "gender", _gender },
							{ "faceRectangle", _rectangle.toAnyValue() }
						});
					}

					void CVImageFace::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_age = b.get<double>("age");
						_gender = b.get<std::string>("gender");
						_rectangle = b.get<CVImageFaceRectangle>("faceRectangle");
					}

				}
			}
		}
	}
}
