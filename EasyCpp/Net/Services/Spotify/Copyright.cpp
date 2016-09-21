#include "Copyright.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Copyright::~Copyright()
				{
				}

				const std::string & Copyright::getText() const
				{
					return _text;
				}

				const std::string & Copyright::getType() const
				{
					return _type;
				}

				AnyValue Copyright::toAnyValue() const
				{
					return Bundle({
						{ "text", _text },
						{ "type", _type }
					});
				}

				void Copyright::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_text = b.get<std::string>("text");
					_type = b.get<std::string>("type");
				}

				Copyright::Copyright()
				{

				}

			}
		}
	}
}