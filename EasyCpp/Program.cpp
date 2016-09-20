#include "Program.h"

#ifdef _WIN32
#include <Windows.h>
#include "StringAlgorithm.h"
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <cstring>
#endif
#include <atomic>

namespace EasyCpp
{
#ifdef _WIN32
	class Program::Impl
	{
	public:
		Impl();
		~Impl();

		void open(const std::string& path, const args_t& args, const env_t& env);
		void kill();
		void wait();
		bool isAlive();

		VFS::OutputStreamPtr getStdin();
		VFS::InputStreamPtr getStdout();
		VFS::InputStreamPtr getStderr();

		uint32_t getExitCode() const;
		uint64_t getHandle() const;
	private:
		static void HandleDtor(HANDLE* handle);
		static std::string LastError();
		typedef std::unique_ptr<HANDLE, void(*)(HANDLE*)> HANDLEPtr;
		VFS::OutputStreamPtr _stdin;
		VFS::InputStreamPtr _stdout;
		VFS::InputStreamPtr _stderr;

		HANDLEPtr _h_process;

		class HandleInputStream : public VFS::InputStream
		{
		public:
			HandleInputStream(HANDLEPtr handle);
			virtual ~HandleInputStream();
			virtual std::vector<uint8_t> read(size_t len) override;
			virtual uint64_t bytesRead() override;
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		private:
			HANDLEPtr _handle;
			std::atomic<uint64_t> _read;
		};
		class HandleOutputStream : public VFS::OutputStream
		{
		public:
			HandleOutputStream(HANDLEPtr handle);
			virtual ~HandleOutputStream();
			virtual size_t write(const std::vector<uint8_t>& data) override;
			virtual uint64_t bytesWritten() override;
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		private:
			HANDLEPtr _handle;
			std::atomic<uint64_t> _written;
		};
	};
#elif defined(__linux__)
	class Program::Impl
	{
	public:
		Impl();
		~Impl();

		void open(const std::string& path, const args_t& args, const env_t& env);
		void kill();
		void wait();
		bool isAlive();

		VFS::OutputStreamPtr getStdin();
		VFS::InputStreamPtr getStdout();
		VFS::InputStreamPtr getStderr();

		uint32_t getExitCode() const;
		uint64_t getHandle() const;
	private:
		pid_t _child_pid;

		VFS::OutputStreamPtr _stdin;
		VFS::InputStreamPtr _stdout;
		VFS::InputStreamPtr _stderr;

		class HandleInputStream : public VFS::InputStream
		{
		public:
			HandleInputStream(int fd);
			virtual ~HandleInputStream();
			virtual std::vector<uint8_t> read(size_t len) override;
			virtual uint64_t bytesRead() override;
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		private:
			int _fd;
			std::atomic<uint64_t> _read;
		};
		class HandleOutputStream : public VFS::OutputStream
		{
		public:
			HandleOutputStream(int fd);
			virtual ~HandleOutputStream();
			virtual size_t write(const std::vector<uint8_t>& data) override;
			virtual uint64_t bytesWritten() override;
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		private:
			int _fd;
			std::atomic<uint64_t> _written;
		};
	};
#endif
	Program::Program()
		: _impl(std::make_unique<Impl>())
	{

	}

	Program::Program(const std::string & path)
		: Program(path, std::vector<std::string>())
	{
	}

	Program::Program(const std::string & path, const env_t & env)
		: Program(path, std::vector<std::string>(), env)
	{
	}

	Program::Program(const std::string & path, const args_t & args, const env_t & env)
		: Program()
	{
		this->open(path, args, env);
	}

	Program::~Program()
	{
	}

	void Program::open(const std::string & path, const args_t & args, const env_t & env)
	{
		_impl->open(path, args, env);
	}

	void Program::kill()
	{
		_impl->kill();
	}

	void Program::wait()
	{
		_impl->wait();
	}

	bool Program::isAlive()
	{
		return _impl->isAlive();
	}

	VFS::OutputStreamPtr Program::getStdin()
	{
		return _impl->getStdin();
	}

	VFS::InputStreamPtr Program::getStdout()
	{
		return _impl->getStdout();
	}

	VFS::InputStreamPtr Program::getStderr()
	{
		return _impl->getStderr();
	}

	uint32_t Program::getExitCode() const
	{
		return _impl->getExitCode();
	}

	uint64_t Program::getHandle() const
	{
		return _impl->getHandle();
	}

#ifdef _WIN32

	Program::Impl::Impl()
		:_h_process(NULL, &Impl::HandleDtor)
	{

	}

	Program::Impl::~Impl()
	{

	}

	void Program::Impl::open(const std::string & path, const args_t & args, const env_t & env)
	{
		auto child_stdout_write = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);
		auto child_stdout_read = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);
		auto child_stderr_write = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);
		auto child_stderr_read = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);
		auto child_stdin_write = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);
		auto child_stdin_read = HANDLEPtr(new HANDLE(NULL), &Impl::HandleDtor);

		SECURITY_ATTRIBUTES security_attributes;
		STARTUPINFOA startup_info;
		PROCESS_INFORMATION process_information;

		memset(&security_attributes, 0x00, sizeof(security_attributes));
		memset(&startup_info, 0x00, sizeof(startup_info));
		memset(&process_information, 0x00, sizeof(process_information));

		security_attributes.nLength = sizeof(security_attributes);
		startup_info.cb = sizeof(startup_info);

		security_attributes.bInheritHandle = TRUE;
		security_attributes.lpSecurityDescriptor = NULL;

		// Setup pipes
		if (!CreatePipe(child_stdout_read.get(), child_stdout_write.get(), &security_attributes, 0))
		{
			throw std::runtime_error("Failed to create pipe: stdout");
		}
		if (!CreatePipe(child_stdin_read.get(), child_stdin_write.get(), &security_attributes, 0))
		{
			throw std::runtime_error("Failed to create pipe: stdin");
		}
		if (!CreatePipe(child_stderr_read.get(), child_stderr_write.get(), &security_attributes, 0))
		{
			throw std::runtime_error("Failed to create pipe: stderr");
		}

		startup_info.dwFlags = STARTF_USESTDHANDLES;
		startup_info.hStdOutput = *child_stdout_write;
		startup_info.hStdError = *child_stderr_write;
		startup_info.hStdInput = *child_stdin_read;

		std::string cmdline = path + " " + implode<std::string>(" ", args);
		if (CreateProcessA(path.c_str(), (char*)cmdline.data(), 0, 0, TRUE, 0, 0, 0, &startup_info, &process_information) == FALSE)
		{
			throw std::runtime_error("CreateProcess failed: " + LastError());
		}
		else {
			CloseHandle(process_information.hThread);
			_h_process = HANDLEPtr(new HANDLE(process_information.hProcess), &Impl::HandleDtor);
		}

		_stdout = std::make_shared<HandleInputStream>(std::move(child_stdout_read));
		_stderr = std::make_shared<HandleInputStream>(std::move(child_stderr_read));
		_stdin = std::make_shared<HandleOutputStream>(std::move(child_stdin_write));
	}

	void Program::Impl::kill()
	{
		if (TerminateProcess(*_h_process, (UINT)-1))
		{
			// TerminateProcess is asynchronous
			this->wait();
		}
	}

	void Program::Impl::wait()
	{
		if (isAlive())
		{
			DWORD status = WaitForSingleObject(*_h_process, INFINITE);
			while (status != WAIT_OBJECT_0) {
				if (status != WAIT_TIMEOUT)
					throw std::runtime_error("Failed to determine status");
				status = WaitForSingleObject(*_h_process, INFINITE);
			}
		}
	}

	bool Program::Impl::isAlive()
	{
		DWORD status = WaitForSingleObject(*_h_process, 0);
		if (status == WAIT_OBJECT_0)
			return false;
		else if (status == WAIT_TIMEOUT)
			return true;
		else throw std::runtime_error("Failed to determine status");
	}

	uint32_t Program::Impl::getExitCode() const
	{
		DWORD code;
		if (GetExitCodeProcess(*_h_process, &code)) {
			return code;
		}
		else throw std::runtime_error("Failed to determine exit code");
	}

	uint64_t Program::Impl::getHandle() const
	{
		return (uint64_t)*_h_process;
	}

	void Program::Impl::HandleDtor(HANDLE* handle)
	{
		if (handle)
		{
			if (*handle != NULL)
				CloseHandle(*handle);
			delete handle;
		}
	}

	std::string Program::Impl::LastError()
	{
		LPTSTR lpMsgBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, lpMsgBuf, (lstrlen((LPCTSTR)lpMsgBuf)), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, lpMsgBuf, (lstrlen((LPCTSTR)lpMsgBuf)), &strTo[0], size_needed, NULL, NULL);

		return trim(strTo) + " (" + std::to_string(dw) + ")";
	}

	Program::Impl::HandleInputStream::HandleInputStream(HANDLEPtr handle)
		:_handle(std::move(handle)), _read(0)
	{

	}

	Program::Impl::HandleInputStream::~HandleInputStream()
	{

	}

	std::vector<uint8_t> Program::Impl::HandleInputStream::read(size_t len)
	{
		if (len > MAXDWORD)
			len = MAXDWORD;
		std::vector<uint8_t> res;
		res.resize(len);
		DWORD read = 0;
		if (ReadFile(*_handle, res.data(), (DWORD)res.size(), &read, NULL) == FALSE)
			throw std::runtime_error("Failed to read pipe " + Impl::LastError());
		_read.fetch_add(read);
		res.resize(read);
		return res;
	}

	Program::Impl::HandleOutputStream::HandleOutputStream(HANDLEPtr handle)
		:_handle(std::move(handle)), _written(0)
	{

	}

	Program::Impl::HandleOutputStream::~HandleOutputStream()
	{

	}

	size_t Program::Impl::HandleOutputStream::write(const std::vector<uint8_t>& data)
	{
		DWORD written = 0;
		if (WriteFile(*_handle, data.data(), (DWORD)std::min<size_t>(data.size(), MAXDWORD), &written, NULL) == FALSE)
			throw std::runtime_error("Failed to write pipe " + Impl::LastError());
		_written.fetch_add(written);
		return written;
	}

#elif defined(__linux__)
	Program::Impl::Impl()
		:_child_pid(0)
	{
	}

	Program::Impl::~Impl()
	{
		if (_child_pid != 0) {
			// Clean up resources
			siginfo_t info;
			memset(&info, 0x00, sizeof(siginfo_t));
			waitid(P_PID, _child_pid, &info, WEXITED | WNOHANG);
		}
	}

	void Program::Impl::open(const std::string & path, const args_t & args, const env_t & env)
	{
		// 4 pipes
		// Stdin, Stdout, Stderr
		// Status pipe set to close on exec in child, writes exec result on failure, closed otherwise
		// [0] = read [1] = write
		int child_stdout[2];
		int child_stdin[2];
		int child_stderr[2];
		int child_exec_check[2];

		if (pipe(child_stdout) == -1)
			throw std::runtime_error("pipe failed");
		if (pipe(child_stdin) == -1)
			throw std::runtime_error("pipe failed");
		if (pipe(child_stderr) == -1)
			throw std::runtime_error("pipe failed");
		if (pipe(child_exec_check) == -1)
			throw std::runtime_error("pipe failed");

		// Split
		int fresult = fork();
		if (fresult < 0) {
			// Error, should never happen
			throw std::runtime_error("Failed to call fork");
		} else if(fresult == 0) {
			// Child flow
			// stdin write, stderr read, stdout read, check read ends
			close(child_stdin[1]);
			close(child_stdout[0]);
			close(child_stderr[0]);
			close(child_exec_check[0]);
			// Duplicate pipe to stdin
			dup2(child_stdin[0], STDIN_FILENO);
			dup2(child_stdout[1], STDOUT_FILENO);
			dup2(child_stderr[1], STDERR_FILENO);
			// We move check_fd to fd(4) to make closing all other fds easier
			dup2(child_exec_check[1], 4);
			// Enable close on exec
			fcntl(child_exec_check[1], F_SETFD, FD_CLOEXEC);

			// Close all fd's larger as 4
			for (int i = 5; i < getdtablesize(); i++)
			{
				close(i);
			}

			// Setup argv and environment arrays
			char** argv = new char*[args.size() + 2];
			argv[args.size()] = NULL;
			argv[0] = path.c_str();
			for (size_t i=0; i< args.size(); i++)
			{
				argv[i+1] = new char[args[i].size() + 1];
				memcpy(argv[i+1], args[i].c_str(), args.size() + 1);
			}
			size_t env_cnt = 0;
			for (auto& e : env)
			{
				env_cnt++;
			}
			char** envp = new char*[env_cnt + 1];
			envp[env_cnt] = NULL;
			for (auto& e : env)
			{
				env_cnt--;
				size_t size = e.first.size() + e.second.size() + 2;
				envp[env_cnt] = new char[size];
				memcpy(envp[env_cnt], e.first.data(), e.first.size());
				envp[env_cnt][e.first.size()] = '=';
				memcpy(&envp[env_cnt][e.first.size() + 1], e.second.data(), e.second.size() + 1);
			}

			// execute new process
			// On success check_descriptor is closed,
			// On error  we send errno via check_descriptor and exit
			execve(path.c_str(), argv, envp);
			int terrno = errno;
			int res = write(4, &terrno, sizeof(terrno));
			_exit(-1);
		} else {
			// Parent flow
			_child_pid = fresult;
			// stdin read, stderr write, stdout write, check write ends
			close(child_stdin[0]);
			close(child_stdout[1]);
			close(child_stderr[1]);
			close(child_exec_check[1]);

			// To check if process was successfully started we try to read from child_exec_check.
			// If this fails with broken pipe the process was started successfully
			// Otherwise 4 bytes containing errno are read.
			int terrno = 0;
			ssize_t res = read(child_exec_check[0], &terrno, sizeof(errno));
			if (res == 0) { // execve successfull
				close(child_exec_check[0]);
				// Setup handler streams
				_stdin = std::make_shared<HandleOutputStream>(child_stdin[1]);
				_stdout = std::make_shared<HandleInputStream>(child_stdout[0]);
				_stderr = std::make_shared<HandleInputStream>(child_stderr[0]);
				return;
			}
			else {
				close(child_exec_check[0]);
				close(child_stdin[1]);
				close(child_stdout[0]);
				close(child_stderr[0]);
				throw std::runtime_error("Execve failed: " + std::to_string(terrno));
			}
		}
	}

	void Program::Impl::kill()
	{
		if (isAlive())
			if (!::kill(_child_pid, SIGKILL))
				throw std::runtime_error("Kill failed");
	}

	void Program::Impl::wait()
	{
		siginfo_t info;
		memset(&info, 0x00, sizeof(siginfo_t));
		int res = waitid(P_PID, _child_pid, &info, WEXITED | WNOWAIT);
		if (res == -1)
			throw std::runtime_error("waitid failed");
		if (info.si_pid != _child_pid)
			throw std::runtime_error("Invalid pid value");
	}

	bool Program::Impl::isAlive()
	{
		siginfo_t info;
		memset(&info, 0x00, sizeof(siginfo_t));
		int res = waitid(P_PID, _child_pid, &info, WNOHANG | WEXITED | WNOWAIT);
		if (res == -1)
			throw std::runtime_error("waitid failed");
		if (info.si_pid == 0) return true;
		if (info.si_pid == _child_pid) return false;
		throw std::runtime_error("Invalid pid value");
	}

	uint32_t Program::Impl::getExitCode() const
	{
		siginfo_t info;
		memset(&info, 0x00, sizeof(siginfo_t));
		int res = waitid(P_PID, _child_pid, &info, WNOHANG | WEXITED | WNOWAIT);
		if (res == -1)
			throw std::runtime_error("waitid failed");
		if (info.si_pid == 0) throw std::runtime_error("Still alive");
		if (info.si_pid == _child_pid) return info.si_status;
		throw std::runtime_error("Invalid pid value");
	}

	uint64_t Program::Impl::getHandle() const
	{
		return _child_pid;
	}

	Program::Impl::HandleInputStream::HandleInputStream(int handle)
		:_fd(handle), _read(0)
	{

	}

	Program::Impl::HandleInputStream::~HandleInputStream()
	{
		close(_fd);
	}

	std::vector<uint8_t> Program::Impl::HandleInputStream::read(size_t len)
	{
		if (len > SSIZE_MAX)
			len = SSIZE_MAX;
		std::vector<uint8_t> res;
		res.resize(len);
		ssize_t bread = ::read(_fd, (void*)res.data(), res.size());
		_read.fetch_add(bread);
		res.resize(bread);
		return res;
	}

	Program::Impl::HandleOutputStream::HandleOutputStream(int handle)
		:_fd(handle), _written(0)
	{

	}

	Program::Impl::HandleOutputStream::~HandleOutputStream()
	{
		close(_fd);
	}

	size_t Program::Impl::HandleOutputStream::write(const std::vector<uint8_t>& data)
	{
		size_t len = data.size();
		if (len > SSIZE_MAX)
			len = SSIZE_MAX;
		ssize_t res = ::write(_fd, data.data(), len);
		_written.fetch_add(res);
		return res;
	}

#endif
	VFS::OutputStreamPtr Program::Impl::getStdin()
	{
		return _stdin;
	}

	VFS::InputStreamPtr Program::Impl::getStdout()
	{
		return _stdout;
	}

	VFS::InputStreamPtr Program::Impl::getStderr()
	{
		return _stderr;
	}

	uint64_t Program::Impl::HandleInputStream::bytesRead()
	{
		return _read.load();
	}

	bool Program::Impl::HandleInputStream::isGood()
	{
		return true;
	}

	uint64_t Program::Impl::HandleInputStream::tell()
	{
		return _read.load();
	}

	void Program::Impl::HandleInputStream::seek(uint64_t pos, seek_origin_t origin)
	{

	}

	bool Program::Impl::HandleInputStream::canSeek()
	{
		return false;
	}

	uint64_t Program::Impl::HandleOutputStream::bytesWritten()
	{
		return _written.load();
	}

	bool Program::Impl::HandleOutputStream::isGood()
	{
		return true;
	}

	uint64_t Program::Impl::HandleOutputStream::tell()
	{
		return _written.load();
	}

	void Program::Impl::HandleOutputStream::seek(uint64_t pos, seek_origin_t origin)
	{

	}

	bool Program::Impl::HandleOutputStream::canSeek()
	{
		return false;
	}

}