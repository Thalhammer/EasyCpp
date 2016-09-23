#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include "FullAlbum.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT SavedAlbum : public Serialize::Serializable
				{
				public:
					SavedAlbum();
					virtual ~SavedAlbum();

					const std::string& getAddedAt() const;
					const FullAlbum& getAlbum() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _added_at;
					FullAlbum _album;
				};
			}
		}
	}
}
