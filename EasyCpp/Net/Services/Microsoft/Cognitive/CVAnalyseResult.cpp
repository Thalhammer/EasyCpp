#include "CVAnalyseResult.h"
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
					CVAnalyseResult::CVAnalyseResult()
					{
					}

					CVAnalyseResult::~CVAnalyseResult()
					{
					}

					const std::vector<CVImageFace>& CVAnalyseResult::getFaces() const
					{
						return _faces;
					}

					const CVImageType & CVAnalyseResult::getImageType() const
					{
						return _imagetype;
					}

					const CVImageColor & CVAnalyseResult::getImageColor() const
					{
						return _imagecolor;
					}

					const CVImageAdult & CVAnalyseResult::getAdult() const
					{
						return _imageadult;
					}

					const CVImageDescription & CVAnalyseResult::getDescription() const
					{
						return _imagedescription;
					}

					const std::vector<CVImageTag>& CVAnalyseResult::getTags() const
					{
						return _tags;
					}

					const std::vector<CVImageCategory>& CVAnalyseResult::getCategories() const
					{
						return _categories;
					}

					const CVImageMetadata & CVAnalyseResult::getMetadata() const
					{
						return _metadata;
					}

					const std::string & CVAnalyseResult::getRequestId() const
					{
						return _request_id;
					}

					AnyValue CVAnalyseResult::toAnyValue() const
					{
						return Bundle({
							{ "metadata", _metadata.toAnyValue() },
							{ "requestId", _request_id },
							{ "faces", toAnyArraySerialize(_faces) },
							{ "imageType", _imagetype.toAnyValue() },
							{ "color", _imagecolor.toAnyValue() },
							{ "adult", _imageadult.toAnyValue() },
							{ "description", _imagedescription.toAnyValue() },
							{ "tags", toAnyArraySerialize(_tags) },
							{ "categories", toAnyArraySerialize(_categories) }
						});
					}

					void CVAnalyseResult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_metadata = b.get<CVImageMetadata>("metadata");
						_request_id = b.get<std::string>("requestId");
						if (b.isSet("faces"))
							_faces = fromAnyArray<CVImageFace>(b.get("faces"));
						if (b.isSet("imageType"))
							_imagetype = b.get<CVImageType>("imageType");
						if (b.isSet("color"))
							_imagecolor = b.get<CVImageColor>("color");
						if (b.isSet("adult"))
							_imageadult = b.get<CVImageAdult>("adult");
						if (b.isSet("description"))
							_imagedescription = b.get<CVImageDescription>("description");
						if (b.isSet("tags"))
							_tags = fromAnyArray<CVImageTag>(b.get("tags"));
						if (b.isSet("categories"))
							_categories = fromAnyArray<CVImageCategory>(b.get("categories"));
					}
				}
			}
		}
	}
}