#pragma once
#include "../DllExport.h"
#include "../VFS/InputOutputStream.h"
#include "../Bundle.h"
#include "../DynamicObject.h"
#include <string>
#include <vector>
#include "URI.h"

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT WebClient: public DynamicObject
		{
		public:
			void DownloadFile(const std::string& url, VFS::OutputStreamPtr stream);
			std::string Download(const std::string& url);
			std::string Upload(const std::string& url, VFS::InputStreamPtr stream);
			std::string Upload(const std::string& url, const std::string& data);
			std::string Upload(const std::string& url, const Bundle& b);

			void setBaseAddress(const URI& base);
			URI getBaseAddress();

			void setHeaders(const Bundle& headers);
			Bundle getHeaders();

			void setUserAgent(const std::string& ua);
			std::string getUserAgent();

			void setCredentials(const std::string& user, const std::string& pass);

			Bundle getResponseHeaders();

			// Geerbt von DynamicObject
			virtual AnyValue getProperty(const std::string & name) override;
			virtual std::vector<std::string> getProperties() override;
			virtual void setProperty(const std::string & name, AnyValue value) override;
			virtual AnyValue callFunction(const std::string & name, const std::vector<AnyValue>& params) override;
			virtual std::vector<std::string> getFunctions() override;
		private:
			std::string _username;
			std::string _password;
			std::string _user_agent;

			URI _base_uri;
			Bundle _headers;

			Bundle _response_headers;
		};
	}
}