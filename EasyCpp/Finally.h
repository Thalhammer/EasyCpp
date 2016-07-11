#pragma once
#include <functional>

namespace EasyCpp
{
	class Finally
	{
	public:

		Finally(std::function<void()> fn)
			:_fn(fn)
		{
		}

		virtual ~Finally()
		{
			if (_fn)
				_fn();
		}

		void dismiss()
		{
			_fn = nullptr;
		}
	private:
		std::function<void()> _fn;
	};
}