#pragma once
#include "CVImageFace.h"
#include "CVImageMetadata.h"
#include "CVImageType.h"
#include "CVImageColor.h"
#include "CVImageAdult.h"
#include "CVImageDescription.h"
#include "CVImageTag.h"
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
					class DLL_EXPORT CVAnalyseResult : public Serialize::Serializable
					{
					public:
						CVAnalyseResult();
						virtual ~CVAnalyseResult();

						const std::vector<CVImageFace>& getFaces() const;
						const CVImageType& getImageType() const;
						const CVImageColor& getImageColor() const;
						const CVImageAdult& getAdult() const;
						const CVImageDescription& getDescription() const;
						const std::vector<CVImageTag>& getTags() const;
						const std::vector<CVImageCategory>& getCategories() const;
						const CVImageMetadata& getMetadata() const;
						const std::string& getRequestId() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _request_id;
						std::vector<CVImageFace> _faces;
						CVImageMetadata _metadata;
						CVImageType _imagetype;
						CVImageColor _imagecolor;
						CVImageAdult _imageadult;
						CVImageDescription _imagedescription;
						std::vector<CVImageTag> _tags;
						std::vector<CVImageCategory> _categories;
					};
				}
			}
		}
	}
}
