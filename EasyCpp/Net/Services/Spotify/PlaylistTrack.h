#pragma once
#include "Playlist.h"
#include "Paging.h"
#include "Followers.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT PlaylistTrack : public Serialize::Serializable
				{
				public:
					PlaylistTrack();
					virtual ~PlaylistTrack();

					const std::string& getAddedAt() const;
					const PublicUser& getAddedBy() const;
					bool getIsLocal() const;
					const FullTrack& getTrack() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _added_at;
					PublicUser _added_by;
					bool _is_local;
					FullTrack _track;
				};
			}
		}
	}
}