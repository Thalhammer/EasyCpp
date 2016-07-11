#include "Backtrace.h"
#include <sstream>
#if defined(__linux__)

#elif defined(_WIN32)
#include <Windows.h>
#ifdef _MSC_VER
#pragma warning(disable:4091)
#endif
#include <DbgHelp.h>
#ifdef _MSC_VER
#pragma warning(default:4091)
#endif
#pragma comment(lib, "Dbghelp.lib")
#endif

namespace EasyCpp
{

	Backtrace::Backtrace()
	{
#if defined(__linux__)
		_trace.push_back("Backtrace is not yet supported on this Platform.");
#elif defined(_WIN32)
		CONTEXT ctx;
		RtlCaptureContext(&ctx);
		this->doBacktrace(&ctx);
#endif
	}

#ifdef _WIN32
	Backtrace::Backtrace(void * context)
	{
		this->doBacktrace(context);
	}

	void Backtrace::doBacktrace(void * context)
	{
		HANDLE process = GetCurrentProcess();
		SymInitialize(process, NULL, TRUE);

		// StackWalk64() may modify context record passed to it, so we will
		// use a copy.
		CONTEXT context_record = *((CONTEXT*)context);
		// Initialize stack walking.
		STACKFRAME64 stack_frame;
		memset(&stack_frame, 0, sizeof(stack_frame));
#if defined(_WIN64)
		int machine_type = IMAGE_FILE_MACHINE_AMD64;
		stack_frame.AddrPC.Offset = context_record.Rip;
		stack_frame.AddrFrame.Offset = context_record.Rbp;
		stack_frame.AddrStack.Offset = context_record.Rsp;
#else
		int machine_type = IMAGE_FILE_MACHINE_I386;
		stack_frame.AddrPC.Offset = context_record.Eip;
		stack_frame.AddrFrame.Offset = context_record.Ebp;
		stack_frame.AddrStack.Offset = context_record.Esp;
#endif
		stack_frame.AddrPC.Mode = AddrModeFlat;
		stack_frame.AddrFrame.Mode = AddrModeFlat;
		stack_frame.AddrStack.Mode = AddrModeFlat;

		SYMBOL_INFO  * symbol;
		symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		while (StackWalk64(machine_type,
			GetCurrentProcess(),
			GetCurrentThread(),
			&stack_frame,
			&context_record,
			NULL,
			&SymFunctionTableAccess64,
			&SymGetModuleBase64,
			NULL)) {

			DWORD64 displacement = 0;

			if (SymFromAddr(process, (DWORD64)stack_frame.AddrPC.Offset, &displacement, symbol))
			{
				IMAGEHLP_MODULE64 moduleInfo;
				moduleInfo.SizeOfStruct = sizeof(moduleInfo);

				std::string entry = "";

				if (::SymGetModuleInfo64(process, symbol->ModBase, &moduleInfo))
					entry += moduleInfo.ModuleName + std::string(": ");

				std::stringstream ss;
				ss << std::hex << symbol->Address;
				entry += symbol->Name + std::string(" - 0x") + ss.str();

				_trace.push_back(entry);
			}

		}
	}
#endif

	Backtrace::~Backtrace()
	{
	}

	void Backtrace::print(std::ostream & out) const
	{
		out << this->print();
	}

	std::string Backtrace::print() const
	{
		std::string res = "Backtrace:\n";
		for (auto s : _trace)
			res += s + "\n";
		return res;
	}

	size_t Backtrace::size() const
	{
		return _trace.size();
	}

	std::string Backtrace::at(size_t pos) const
	{
		return _trace.at(pos);
	}

}