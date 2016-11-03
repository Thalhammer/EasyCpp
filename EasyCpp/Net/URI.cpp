#include "URI.h"
#include <regex>
#include "../RuntimeException.h"

namespace EasyCpp
{
	namespace Net
	{
		URI::URI()
		{
			_port = -1;
		}

		URI::URI(const std::string& uri)
			:URI()
		{
			std::regex reg("^(.+):\\/\\/(.+@)?([a-zA-Z.\\-0-9]+)(:\\d{1,5})?([^?\\n\\#]*)(\\?[^#\\n]*)?(\\#.*)?$");
			std::smatch match;
			if (std::regex_match(uri, match, reg))
			{
				setScheme(match[1].str());
				setHostname(match[3].str());
				setPath(URLDecode(match[5].str()));
				// Optional parts
				if (match[2].matched)
				{
					auto str = match[2].str();
					auto pos = str.find(':');
					if (pos != str.npos)
					{
						setUser(str.substr(0, pos));
						setPassword(str.substr(pos + 1, str.length() - pos - 2));
					}
					else {
						setUser(str.substr(0, str.length() - 1));
					}
				}
				if (match[4].matched)
				{
					setPort(std::stoul(match[4].str().substr(1)));
				}
				if (match[6].matched)
				{
					setQuery(match[6].str());
				}
				if (match[7].matched)
				{
					setFragment(match[7].str());
				}
			}
			else
				throw RuntimeException("Not a valid uri");
		}


		URI::~URI()
		{
		}

		void URI::setPath(const std::string & path)
		{
			if (path.empty())
			{
				_path = "/";
			}
			else {
				if (path[0] == '/')
					_path = path;
				else _path = "/" + path;
			}
		}

		void URI::setPort(int port)
		{
			if (port < -1 || port > UINT16_MAX)
				throw RuntimeException("Invalid port");
			_port = port;
		}

		std::string URI::str() const
		{
			std::string res = _scheme;
			res += "://";
			if (!_user.empty())
			{
				res += _user;
				if (!_pass.empty())
				{
					res += ":" + _pass;
				}
				res += "@";
			}
			res += _host;
			if (_port != -1)
			{
				res += ":" + std::to_string(_port);
			}
			res += URLEncode(_path, "/");
			res += _query;
			res += _fragment;
			return res;
		}

		std::string URI::URLEncode(const std::string & str, const std::string& ignored)
		{
			std::string res;
			for (char c : str)
			{
				if ((c >= '0'&&c <= '9') || (c >= 'A'&&c <= 'Z') || (c >= 'a'&&c <= 'z') || c == '-' || c == '_' || c == '.' || c == '~' || ignored.find(c) != std::string::npos)
					res += c;
				else {
					res += '%';
					char out[3];
					snprintf(out, 3, "%2X", (int)c);
					res += out;
				}
			}
			return res;
		}

		std::string URI::URLDecode(const std::string & str)
		{
			std::string res;
			for (size_t i = 0; i < str.length(); i++)
			{
				char c = str[i];
				if (c == '%')
				{
					std::string name = str.substr(i + 1, 2);
					int ch = 0;
#ifdef _MSC_VER
					sscanf_s(name.c_str(), "%2X", &ch);
#else
					sscanf(name.c_str(), "%2X", &ch);
#endif
					res += (char)ch;
					i += 2;
				}
				else res += c;
			}
			return res;
		}
	}
}