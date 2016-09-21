#pragma once
#include "Artist.h"
#include "Followers.h"
#include "Image.h"
#include <vector>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT FullArtist : public Artist
				{
				public:
					FullArtist();
					virtual ~FullArtist();

					const Followers& getFollowers() const;
					const std::vector<std::string>& getGenres() const;
					const std::vector<Image>& getImages() const;
					int getPopularity() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					Followers _followers;
					std::vector<std::string> _genres;
					std::vector<Image> _images;
					int _popularity;
				};
			}
		}
	}
}
