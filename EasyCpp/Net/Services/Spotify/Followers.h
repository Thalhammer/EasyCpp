#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Followers : public Serialize::Serializable
				{
				public:
					Followers();
					virtual ~Followers();
					const std::string& getHref() const;
					uint64_t getTotal() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _href;
					uint64_t _total;
				};

			}
		}
	}
}
