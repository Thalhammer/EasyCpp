#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include "FullTrack.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT SavedTrack : public Serialize::Serializable
				{
				public:
					SavedTrack();
					virtual ~SavedTrack();

					const std::string& getAddedAt() const;
					const FullTrack& getTrack() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _added_at;
					FullTrack _track;
				};
			}
		}
	}
}
