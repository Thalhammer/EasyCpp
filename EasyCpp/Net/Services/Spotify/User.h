#pragma once
#include "PublicUser.h"
#include "ProductType.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT User : public PublicUser
				{
				public:
					User();
					virtual ~User();
					const std::string& getBirthday() const;
					const std::string& getCountry() const;
					const std::string& getEMail() const;
					ProductType getProduct() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _birthday;
					std::string _country;
					std::string _email;
					ProductType _product;
				};
			}
		}
	}
}
