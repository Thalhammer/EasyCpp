#pragma once
#include "InputStream.h"
#include "OutputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		class InputOutputStream : public virtual InputStream, public virtual OutputStream
		{

		};
		typedef std::shared_ptr<InputOutputStream> InputOutputStreamPtr;
	}
}
