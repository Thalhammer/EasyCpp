#include "User.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				User::~User()
				{
				}

				const std::string & User::getBirthday() const
				{
					return _birthday;
				}

				const std::string & User::getCountry() const
				{
					return _country;
				}

				const std::string & User::getEMail() const
				{
					return _email;
				}

				ProductType User::getProduct() const
				{
					return _product;
				}

				User::User()
				{
				}

				AnyValue User::toAnyValue() const
				{
					Bundle b = PublicUser::toAnyValue().as<Bundle>();
					b.set("birthdate", _birthday);
					b.set("country", _country);
					b.set("email", _email);
					b.set("product", _product == ProductType::PREMIUM ? "premium" : "free");
					return b;
				}

				void User::fromAnyValue(const AnyValue & state)
				{
					PublicUser::fromAnyValue(state);
					Bundle bundle = state.as<Bundle>();
					_birthday = bundle.isSet("birthdate") ? bundle.get<std::string>("birthdate") : "";
					_country = bundle.get<std::string>("country");
					_email = bundle.isSet("email") ? bundle.get<std::string>("email") : "";
					if (bundle.isSet("product"))
						_product = bundle.get<std::string>("product") == "premium" ? ProductType::PREMIUM : ProductType::FREE;
				}
			}
		}
	}
}