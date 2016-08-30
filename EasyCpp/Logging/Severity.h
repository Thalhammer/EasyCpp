#pragma once

namespace EasyCpp
{
	namespace Logging
	{
		enum class Severity
		{
			Emergency = 0,
			Alert,
			Critical,
			Error,
			Warning,
			Notice,
			Informational,
			Debug,

			MAX = Debug,
			MIN = Emergency
		};
	}
}