#pragma once
#include "../DllExport.h"
#include <string>
#include <map>

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT URI
		{
		public:
			URI();
			URI(const std::string& uri);
			~URI();

			const std::string& getScheme() const { return _scheme; }
			const std::string& getUser() const { return _user; }
			const std::string& getPassword() const { return _pass; }
			const std::string& getHostname() const { return _host; }
			const std::string& getPath() const { return _path; }
			const std::string& getQuery() const { return _query; }
			const std::string& getFragment() const { return _fragment; }
			int getPort() const { return _port; }
			const std::multimap<std::string, std::string>& getParams() const { return _params; }


			void setScheme(const std::string& scheme) { _scheme = scheme; }
			void setUser(const std::string& user) { _user = user; }
			void setPassword(const std::string& pass) { _pass = pass; }
			void setHostname(const std::string& host) { _host = host; }
			void setPath(const std::string& path);
			void setQuery(const std::string& query) { _query = query; }
			void setFragment(const std::string& fragment) { _fragment = fragment; }
			void setPort(int port);

			std::string str() const;

			static std::string URLEncode(const std::string& str, const std::string& ignored = "");
			static std::string URLDecode(const std::string& str);
			static std::multimap<std::string, std::string> ParseURLParams(const std::string& str);
		private:
			std::string _scheme;
			std::string _user;
			std::string _pass;
			std::string _host;
			std::string _path;
			std::string _query;
			std::string _fragment;
			int _port;
			std::multimap<std::string, std::string> _params;
		};
	}
}