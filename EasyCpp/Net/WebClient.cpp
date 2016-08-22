#include "WebClient.h"
#include "Curl.h"
#include "../RuntimeException.h"
#include "../StringAlgorithm.h"
#include <cstring>

namespace EasyCpp
{
	namespace Net
	{
		AnyValue WebClient::getProperty(const std::string & name)
		{
			if (name == "headers") {
				return this->getHeaders();
			}
			else if (name == "base_address") {
				return this->getBaseAddress().str();
			}
			else if (name == "response_headers") {
				return this->getResponseHeaders();
			}
			else if (name == "auth_user" || name == "auth_password") {
				return AnyValue();
			}
			else if (name == "user_agent") {
				return this->getUserAgent();
			}
			else return AnyValue();
		}

		std::vector<std::string> WebClient::getProperties()
		{
			return{
				"headers",
				"base_address",
				"response_headers",
				"auth_user",
				"auth_password",
				"user_agent"
			};
		}

		void WebClient::setProperty(const std::string & name, AnyValue value)
		{
			if (name == "headers") {
				this->setHeaders(value.as<Bundle>());
			}
			else if (name == "base_address") {
				this->setBaseAddress(URI(value.as<std::string>()));
			}
			else if (name == "response_headers") {
				// Read only
			}
			else if (name == "auth_user") {
				_password = value.as<std::string>();
			}
			else if (name == "auth_password") {
				_username = value.as<std::string>();
			}
			else if (name == "user_agent") {
				this->setUserAgent(value.as<std::string>());
			}
		}

		AnyValue WebClient::callFunction(const std::string & name, const std::vector<AnyValue>& params)
		{
			if (name == "DownloadFile") {
				if (params.size() != 2) {
					throw RuntimeException("Missing parameter");
				}
				this->DownloadFile(params[0].as<std::string>(), params[1].as<VFS::OutputStreamPtr>());
			}
			else if (name == "Download") {
				if (params.size() != 1) {
					throw RuntimeException("Missing parameter");
				}
				return this->Download(params[0].as<std::string>());
			}
			else if (name == "Upload") {
				if (params.size() != 2) {
					throw RuntimeException("Missing parameter");
				}
				if (params[1].isType<VFS::InputStreamPtr>()) {
					return this->Upload(params[0].as<std::string>(), params[1].as<VFS::InputStreamPtr>());
				}
				else if (params[1].isType<Bundle>()) {
					return this->Upload(params[0].as<std::string>(), params[1].as<Bundle>());
				}
				else if (params[1].isConvertibleTo<std::string>()) {
					return this->Upload(params[0].as<std::string>(), params[1].as<std::string>());
				}
				else throw RuntimeException("No valid function to match parameters");
			}
			else if (name == "setBaseAddress") {
				if (params.size() != 1) {
					throw RuntimeException("Missing parameter");
				}
				this->setBaseAddress(params[0].as<std::string>());
				return AnyValue();
			}
			else if (name == "getBaseAddress") {
				return this->getBaseAddress();
			}
			else if (name == "setHeaders") {
				if (params.size() != 1) {
					throw RuntimeException("Missing parameter");
				}
				this->setHeaders(params[0].as<Bundle>());
				return AnyValue();
			}
			else if (name == "getHeaders") {
				return this->getHeaders();
			}
			else if (name == "setUserAgent") {
				if (params.size() != 1) {
					throw RuntimeException("Missing parameter");
				}
				this->setUserAgent(params[0].as<std::string>());
				return AnyValue();
			}
			else if (name == "getUserAgent") {
				return this->getUserAgent();
			}
			else if (name == "setCredentials") {
				if (params.size() != 2) {
					throw RuntimeException("Missing parameter");
				}
				this->setCredentials(params[0].as<std::string>(), params[1].as<std::string>());
				return AnyValue();
			}
			else if (name == "getResponseHeaders") {
				return this->getResponseHeaders();
			}
			return AnyValue();
		}

		std::vector<std::string> WebClient::getFunctions()
		{
			return{
				"DownloadFile",
				"Download",
				"Upload",
				"setBaseAddress",
				"getBaseAddress",
				"setHeaders",
				"getHeaders",
				"setUserAgent",
				"getUserAgent",
				"setCredentials",
				"getResponseHeaders"
			};
		}

		void WebClient::DownloadFile(const std::string & url, VFS::OutputStreamPtr stream)
		{
			Curl curl;
			curl.setURL(URI(_base_uri.str() + url).str());
			curl.setWriteFunction([stream](char* data, uint64_t len) {
				return stream->write(std::vector<uint8_t>(data, data + len));
			});
			if (_user_agent != "") {
				curl.setUserAgent(_user_agent);
			}
			else {
				curl.setUserAgent("libcurl-agent/1.0");
			}
			if (_username != "" || _password != "") {
				curl.setUsername(_username);
				curl.setPassword(_password);
			}
			std::multimap<std::string, std::string> headers;
			for (auto& e : _headers) headers.insert({ e.first, e.second.as<std::string>() });
			curl.setHeaders(headers);
			curl.setHeaderFunction([this](std::string header) {
				size_t pos = header.find(':');
				if (pos != std::string::npos) {
					std::string key = header.substr(0, pos);
					std::string value = header.substr(pos + 1);
					_response_headers.set(trim(key), trim(value));
				}
			});
			curl.perform();
		}

		std::string WebClient::Download(const std::string & url)
		{
			std::string result;
			Curl curl;
			curl.setURL(URI(_base_uri.str() + url).str());
			curl.setOutputString(result);
			if (_user_agent != "") {
				curl.setUserAgent(_user_agent);
			}
			else {
				curl.setUserAgent("libcurl-agent/1.0");
			}
			if (_username != "" || _password != "") {
				curl.setUsername(_username);
				curl.setPassword(_password);
			}
			std::multimap<std::string, std::string> headers;
			for (auto& e : _headers) headers.insert({ e.first, e.second.as<std::string>() });
			curl.setHeaders(headers);
			curl.setHeaderFunction([this](std::string header) {
				size_t pos = header.find(':');
				if (pos != std::string::npos) {
					std::string key = header.substr(0, pos);
					std::string value = header.substr(pos + 1);
					_response_headers.set(trim(key), trim(value));
				}
			});
			curl.perform();
			return result;
		}

		std::string WebClient::Upload(const std::string & url, VFS::InputStreamPtr stream)
		{
			std::string result;
			Curl curl;
			curl.setURL(URI(_base_uri.str() + url).str());
			curl.setOutputString(result);
			curl.setReadFunction([stream](char* data, uint64_t len) {
				if (!stream->isGood()) return size_t(0);
				auto read = stream->read((size_t)std::min<uint64_t>(size_t(-1),len));
				memcpy(data, read.data(), read.size());
				return read.size();
			});
			if (_user_agent != "") {
				curl.setUserAgent(_user_agent);
			}
			else {
				curl.setUserAgent("libcurl-agent/1.0");
			}
			if (_username != "" || _password != "") {
				curl.setUsername(_username);
				curl.setPassword(_password);
			}
			std::multimap<std::string, std::string> headers;
			for (auto& e : _headers) headers.insert({ e.first, e.second.as<std::string>() });
			curl.setHeaders(headers);
			curl.setHeaderFunction([this](std::string header) {
				size_t pos = header.find(':');
				if (pos != std::string::npos) {
					std::string key = header.substr(0, pos);
					std::string value = header.substr(pos + 1);
					_response_headers.set(trim(key), trim(value));
				}
			});
			curl.perform();
			return result;
		}

		std::string WebClient::Upload(const std::string & url, const std::string & data)
		{
			std::string result;
			Curl curl;
			curl.setURL(URI(_base_uri.str() + url).str());
			curl.setOutputString(result);
			curl.setInputString(data);
			if (_user_agent != "") {
				curl.setUserAgent(_user_agent);
			}
			else {
				curl.setUserAgent("libcurl-agent/1.0");
			}
			if (_username != "" || _password != "") {
				curl.setUsername(_username);
				curl.setPassword(_password);
			}
			std::multimap<std::string, std::string> headers;
			for (auto& e : _headers) headers.insert({ e.first, e.second.as<std::string>() });
			curl.setHeaders(headers);
			curl.setHeaderFunction([this](std::string header) {
				size_t pos = header.find(':');
				if (pos != std::string::npos) {
					std::string key = header.substr(0, pos);
					std::string value = header.substr(pos + 1);
					_response_headers.set(trim(key), trim(value));
				}
			});
			curl.perform();
			return result;
		}

		std::string WebClient::Upload(const std::string & url, const Bundle & b)
		{
			std::vector<std::string> elems;
			for (auto& e : b) {
				elems.push_back(URI::URLEncode(e.first) + "=" + URI::URLEncode(e.second.as<std::string>()));
			}
			return this->Upload(url, implode<std::string>("&", elems));
		}

		void WebClient::setBaseAddress(const URI & base)
		{
			_base_uri = base;
		}

		URI WebClient::getBaseAddress()
		{
			return _base_uri;
		}

		void WebClient::setHeaders(const Bundle & headers)
		{
			_headers = headers;
		}

		Bundle WebClient::getHeaders()
		{
			return _headers;
		}

		void WebClient::setUserAgent(const std::string & ua)
		{
			_user_agent = ua;
		}

		std::string WebClient::getUserAgent()
		{
			return _user_agent;
		}

		void WebClient::setCredentials(const std::string & user, const std::string & pass)
		{
			_username = user;
			_password = pass;
		}

		Bundle WebClient::getResponseHeaders()
		{
			return _response_headers;
		}
	}
}