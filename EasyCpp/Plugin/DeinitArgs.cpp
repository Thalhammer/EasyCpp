#include "DeinitArgs.h"

namespace EasyCpp
{
	namespace Plugin
	{
		void DeinitArgs::cancel(Reason r, const std::string & reason)
		{
			_cancel = true;
			_cancel_reason = r;
			_cancel_reason_str = reason;
		}

		bool DeinitArgs::isCanceled()
		{
			return _cancel;
		}

		DeinitArgs::Reason DeinitArgs::getCancelReason()
		{
			return _cancel_reason;
		}

		std::string DeinitArgs::getCancelString()
		{
			return _cancel_reason_str;
		}
	}
}