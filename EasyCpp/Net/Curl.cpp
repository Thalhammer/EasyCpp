#include "Curl.h"
#include <curl/curl.h>
#include <string>
#include <stdexcept>
#include <map>
#include <cstring>
#include "../StringAlgorithm.h"

namespace EasyCpp
{
	namespace Net
	{
		Curl::Curl()
		{
			_handle = curl_easy_init();
			_error_buffer = (char*)malloc(CURL_ERROR_SIZE);
			reset();
		}

		Curl::~Curl()
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			curl_easy_cleanup(_handle);
			free(_error_buffer);
		}

		void Curl::perform()
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_perform(_handle));
		}

		bool Curl::receive(void * buffer, size_t buflen, size_t & bytes_read)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			CURLcode code = curl_easy_recv(_handle, buffer, buflen, &bytes_read);
			if (code == CURLE_AGAIN)
				return false;
			checkCode(code);
			return true;
		}

		bool Curl::send(void * buffer, size_t buflen, size_t & bytes_send)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_send(_handle, buffer, buflen, &bytes_send));
			return true;
		}

		bool Curl::wait(bool recv, uint64_t timeout_ms)
		{
			curl_socket_t sock = getActiveSocket();

			struct timeval tv;
			fd_set infd, outfd, errfd;
			int res;

			tv.tv_sec = timeout_ms / 1000;
			tv.tv_usec = (timeout_ms % 1000) * 1000;

			FD_ZERO(&infd);
			FD_ZERO(&outfd);
			FD_ZERO(&errfd);

			FD_SET(sock, &errfd); /* always check for error */

			if (recv) {
				FD_SET(sock, &infd);
			}
			else {
				FD_SET(sock, &outfd);
			}

			/* select() returns the number of signalled sockets or -1 */
			res = select(sock + 1, &infd, &outfd, &errfd, &tv);
			return res != 0;
		}

		void Curl::reset()
		{
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				curl_easy_reset(_handle);
			}
			setOption(CURLOPT_NOSIGNAL, true);
			setOption(CURLOPT_ERRORBUFFER, _error_buffer);
			memset(_error_buffer, 0x00, CURL_ERROR_SIZE);
		}

		void Curl::setOption(CurlOption option, void * val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_setopt(_handle, (CURLoption)option, val));
		}

		void Curl::setOption(CurlOption option, bool val)
		{
			setOption(option, (long)(val ? 1 : 0));
		}

		void Curl::setOption(CurlOption option, long val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_setopt(_handle, (CURLoption)option, val));
		}

		void Curl::setOption(CurlOption option, long long val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_setopt(_handle, (CURLoption)option, val));
		}

		void Curl::setOption(CurlOption option, const std::string & val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_setopt(_handle, (CURLoption)option, val.c_str()));
		}

		void Curl::getInfo(CurlInfo info, long & val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_getinfo(_handle, (CURLINFO)info, &val));
		}

		void Curl::getInfo(CurlInfo info, std::string & val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			char* ptr = nullptr;
			checkCode(curl_easy_getinfo(_handle, (CURLINFO)info, &ptr));
			if (ptr != nullptr)
				val = std::string(ptr);
		}

		void Curl::getInfo(CurlInfo info, std::vector<std::string>& val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			struct curl_slist* slist = nullptr;
			checkCode(curl_easy_getinfo(_handle, (CURLINFO)info, &slist));
			val = fromSList(slist);
			curl_slist_free_all(slist);
		}

		void Curl::getInfo(CurlInfo info, double & val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_getinfo(_handle, (CURLINFO)info, &val));
		}

		void Curl::getInfo(CurlInfo info, void ** val)
		{
			std::unique_lock<std::mutex> lck(_handle_lock);
			checkCode(curl_easy_getinfo(_handle, (CURLINFO)info, val));
		}

		void Curl::setVerbose(bool v)
		{
			setOption(CURLOPT_VERBOSE, v);
		}

		void Curl::setHeader(bool v)
		{
			setOption(CURLOPT_HEADER, v);
		}

		void Curl::setNoProgress(bool v)
		{
			setOption(CURLOPT_NOPROGRESS, v);
		}

		void Curl::setWildcardMatch(bool v)
		{
			setOption(CURLOPT_WILDCARDMATCH, v);
		}

		void Curl::setURL(const std::string & url)
		{
			setOption(CURLOPT_URL, url);
		}

		void Curl::setURL(const URI & uri)
		{
			setURL(uri.str());
		}

		void Curl::setPathAsIs(bool v)
		{
			setOption(CURLOPT_PATH_AS_IS, v);
		}

		void Curl::setProtocols(const std::string & protocols)
		{
			setOption(CURLOPT_PROTOCOLS, getProtocolMask(protocols));
		}

		void Curl::setRedirectProtocols(const std::string & protocols)
		{
			setOption(CURLOPT_REDIR_PROTOCOLS, getProtocolMask(protocols));
		}

		void Curl::setDefaultProtocol(const std::string & protocol)
		{
			setOption(CURLOPT_DEFAULT_PROTOCOL, protocol);
		}

		void Curl::setProxy(const std::string & proxy)
		{
			setOption(CURLOPT_PROXY, proxy);
		}

		void Curl::setProxyPort(uint16_t port)
		{
			setOption(CURLOPT_PROXYPORT, (long)port);
		}

		void Curl::setProxyType(ProxyType type)
		{
			long ctype;
			switch (type)
			{
			case ProxyType::HTTP: ctype = CURLPROXY_HTTP; break;
			case ProxyType::HTTP_1_0: ctype = CURLPROXY_HTTP_1_0; break;
			case ProxyType::SOCKS4: ctype = CURLPROXY_SOCKS4; break;
			case ProxyType::SOCKS4A: ctype = CURLPROXY_SOCKS4A; break;
			case ProxyType::SOCKS5: ctype = CURLPROXY_SOCKS5; break;
			case ProxyType::SOCKS5_HOSTNAME: ctype = CURLPROXY_SOCKS5_HOSTNAME; break;
			default:
				ctype = CURLPROXY_HTTP; break;
			}
			setOption(CURLOPT_PROXYTYPE, ctype);
		}

		void Curl::setNoProxy(const std::string & hosts)
		{
			setOption(CURLOPT_NOPROXY, hosts);
		}

		void Curl::setHTTPProxyTunnel(bool v)
		{
			setOption(CURLOPT_HTTPPROXYTUNNEL, v);
		}

		void Curl::setConnectTo(const std::vector<std::string>& list)
		{
#if defined(CURLOPT_CONNECT_TO)
			setOption(CURLOPT_CONNECT_TO, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_connect_to = toSList(list);
			}
			setOption(CURLOPT_CONNECT_TO, _slist_connect_to.get());
#else
			throw std::runtime_error("Not supported by this CURL version");
#endif
		}

		void Curl::setSocks5GSSAPINegotiationProtection(bool v)
		{
			setOption(CURLOPT_SOCKS5_GSSAPI_NEC, v);
		}

		void Curl::setProxyServiceName(const std::string & service)
		{
			setOption(CURLOPT_PROXY_SERVICE_NAME, service);
		}

		void Curl::setAuthenticationServiceName(const std::string & name)
		{
			setOption(CURLOPT_SERVICE_NAME, name);
		}

		void Curl::setInterface(const std::string & str)
		{
			if (str != "")
				setOption(CURLOPT_INTERFACE, str);
			else setOption(CURLOPT_INTERFACE, nullptr);
		}

		void Curl::setLocalPort(uint16_t port)
		{
			setOption(CURLOPT_LOCALPORT, (long)port);
		}

		void Curl::setLocalPortRange(uint16_t range)
		{
			setOption(CURLOPT_LOCALPORTRANGE, (long)range);
		}

		void Curl::setDNSCacheTimeout(long timeout)
		{
			setOption(CURLOPT_DNS_CACHE_TIMEOUT, timeout);
		}

		void Curl::setBufferSize(long size)
		{
			setOption(CURLOPT_BUFFERSIZE, size);
		}

		void Curl::setPort(uint16_t port)
		{
			setOption(CURLOPT_PORT, (long)port);
		}

		void Curl::setTCPFastOpen(bool v)
		{
#if defined(CURLOPT_TCP_FASTOPEN)
			setOption(CURLOPT_TCP_FASTOPEN, v);
#else
			throw std::runtime_error("Not supported by this CURL version");
#endif
		}

		void Curl::setTCPNoDelay(bool v)
		{
			setOption(CURLOPT_TCP_NODELAY, v);
		}

		void Curl::setAddressScope(long scope)
		{
			setOption(CURLOPT_ADDRESS_SCOPE, scope);
		}

		void Curl::setTCPKeepAlive(bool v)
		{
			setOption(CURLOPT_TCP_KEEPALIVE, v);
		}

		void Curl::setTCPKeepAliveIdleTime(long time)
		{
			setOption(CURLOPT_TCP_KEEPIDLE, time);
		}

		void Curl::setTCPKeepAliveInterval(long time)
		{
			setOption(CURLOPT_TCP_KEEPINTVL, time);
		}

		void Curl::setUnixSocketPath(const std::string & path)
		{
			if (path == "")
				setOption(CURLOPT_UNIX_SOCKET_PATH, nullptr);
			else setOption(CURLOPT_UNIX_SOCKET_PATH, path);
		}

		void Curl::setNETRC(bool read, bool required)
		{
			if (read)
			{
				if (required)
					setOption(CURLOPT_NETRC, (long)CURL_NETRC_REQUIRED);
				else setOption(CURLOPT_NETRC, (long)CURL_NETRC_OPTIONAL);
			}
			else setOption(CURLOPT_NETRC, (long)CURL_NETRC_IGNORED);
		}

		void Curl::setNETRCFilename(const std::string & name)
		{
			setOption(CURLOPT_NETRC_FILE, name);
		}

		void Curl::setUsername(const std::string & user)
		{
			setOption(CURLOPT_USERNAME, user);
		}

		void Curl::setPassword(const std::string & pass)
		{
			setOption(CURLOPT_PASSWORD, pass);
		}

		void Curl::setProxyUsername(const std::string & user)
		{
			setOption(CURLOPT_PROXYUSERNAME, user);
		}

		void Curl::setProxyPassword(const std::string & password)
		{
			setOption(CURLOPT_PROXYPASSWORD, password);
		}

		void Curl::setLoginOptions(const std::string & options)
		{
			setOption(CURLOPT_LOGIN_OPTIONS, options);
		}

		void Curl::setHTTPAuth(const std::string & methods)
		{
			long bitmask = 0;
			for (const std::string& str : stringSplit(" ", methods))
			{
				if (str == "basic") bitmask |= CURLAUTH_BASIC;
				else if (str == "digest") bitmask |= CURLAUTH_DIGEST;
				else if (str == "digest_ie") bitmask |= CURLAUTH_DIGEST_IE;
				else if (str == "negotiate") bitmask |= CURLAUTH_NEGOTIATE;
				else if (str == "gssnegotiate") bitmask |= CURLAUTH_GSSNEGOTIATE;
				else if (str == "ntlm") bitmask |= CURLAUTH_NTLM;
				else if (str == "ntlm_wb") bitmask |= CURLAUTH_NTLM_WB;
				else if (str == "any") bitmask |= CURLAUTH_ANY;
				else if (str == "anysafe") bitmask |= CURLAUTH_ANYSAFE;
				else if (str == "only") bitmask |= CURLAUTH_ONLY;
			}
			setOption(CURLOPT_HTTPAUTH, bitmask);
		}

		void Curl::setTLSAuthUsername(const std::string & user)
		{
			setOption(CURLOPT_TLSAUTH_USERNAME, user);
		}

		void Curl::setTLSAuthPassword(const std::string & pass)
		{
			setOption(CURLOPT_TLSAUTH_PASSWORD, pass);
		}

		void Curl::setTLSAuthType(const std::string & type)
		{
			setOption(CURLOPT_TLSAUTH_TYPE, type);
		}

		void Curl::setProxyAuth(const std::string & methods)
		{
			long bitmask = 0;
			for (const std::string& str : stringSplit(" ", methods))
			{
				if (str == "basic") bitmask |= CURLAUTH_BASIC;
				else if (str == "digest") bitmask |= CURLAUTH_DIGEST;
				else if (str == "digest_ie") bitmask |= CURLAUTH_DIGEST_IE;
				else if (str == "negotiate") bitmask |= CURLAUTH_NEGOTIATE;
				else if (str == "gssnegotiate") bitmask |= CURLAUTH_GSSNEGOTIATE;
				else if (str == "ntlm") bitmask |= CURLAUTH_NTLM;
				else if (str == "ntlm_wb") bitmask |= CURLAUTH_NTLM_WB;
				else if (str == "any") bitmask |= CURLAUTH_ANY;
				else if (str == "anysafe") bitmask |= CURLAUTH_ANYSAFE;
				else if (str == "only") bitmask |= CURLAUTH_ONLY;
			}
			setOption(CURLOPT_PROXYAUTH, bitmask);
		}

		void Curl::setSASLIR(bool v)
		{
			setOption(CURLOPT_SASL_IR, v);
		}

		void Curl::setXOAuth2Bearer(const std::string & token)
		{
			setOption(CURLOPT_XOAUTH2_BEARER, token);
		}

		void Curl::setWriteFunction(std::function<uint64_t(char*, uint64_t)> fn)
		{
			_write_fn = fn;
			setOption(CURLOPT_WRITEFUNCTION, (void*)&_s_write_callback);
			setOption(CURLOPT_WRITEDATA, this);
		}

		void Curl::setReadFunction(std::function<uint64_t(char*, uint64_t)> fn)
		{
			_read_fn = fn;
			setOption(CURLOPT_READFUNCTION, (void*)&_s_read_callback);
			setOption(CURLOPT_READDATA, this);
		}

		void Curl::setStreamRestartFunction(std::function<bool()> fn)
		{
			_ioctl_restart_fn = fn;
			setOption(CURLOPT_IOCTLFUNCTION, (void*)&_s_ioctl_callback);
			setOption(CURLOPT_IOCTLDATA, this);
		}

		void Curl::setSeekFunction(std::function<bool(uint64_t, int)> fn)
		{
			_seek_fn = fn;
			setOption(CURLOPT_SEEKFUNCTION, (void*)&_s_seek_callback);
			setOption(CURLOPT_SEEKDATA, this);
		}

		void Curl::setXferInfoFunction(std::function<void(uint64_t, uint64_t, uint64_t, uint64_t)> fn)
		{
			_xferinfo_fn = fn;
			setOption(CURLOPT_XFERINFOFUNCTION, (void*)&_s_xferinfo_callback);
			setOption(CURLOPT_XFERINFODATA, this);
		}

		void Curl::setHeaderFunction(std::function<void(std::string)> fn)
		{
			_header_fn = fn;
			setOption(CURLOPT_HEADERFUNCTION, (void*)&_s_header_callback);
			setOption(CURLOPT_HEADERDATA, this);
		}

		void Curl::setDebugFunction(std::function<void(InfoType, char*, size_t)> fn)
		{
			_debug_fn = fn;
			setOption(CURLOPT_DEBUGFUNCTION, (void*)&_s_debug_callback);
			setOption(CURLOPT_DEBUGDATA, this);
		}

		void Curl::setMatchFunction(std::function<bool(const std::string&pattern, const std::string&string)> fn)
		{
			_match_fn = fn;
			setOption(CURLOPT_FNMATCH_FUNCTION, (void*)&_s_fnmatch_callback);
			setOption(CURLOPT_FNMATCH_DATA, this);
		}

		void Curl::setDebugString(std::string & debug)
		{
			this->setDebugFunction([&debug](auto type, auto data, auto len) {
				if (type == InfoType::Text)
					debug += data;
			});
		}

		void Curl::setFailOnError(bool v)
		{
			setOption(CURLOPT_FAILONERROR, v);
		}

		std::string Curl::getErrorBuffer() const
		{
			return std::string(_error_buffer);
		}

		void Curl::setMailFrom(const std::string & from)
		{
			setOption(CURLOPT_MAIL_FROM, from);
		}

		void Curl::setMailRecipients(const std::vector<std::string> & rcpt)
		{
			setOption(CURLOPT_MAIL_RCPT, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_mail_rcpt = toSList(rcpt);
			}
			setOption(CURLOPT_MAIL_RCPT, _slist_mail_rcpt.get());

		}

		void Curl::setMailAuth(const std::string & auth)
		{
			setOption(CURLOPT_MAIL_AUTH, auth);
		}

		void Curl::setTelnetOptions(const std::vector<std::string> & options)
		{
			setOption(CURLOPT_TELNETOPTIONS, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_telnet_options = toSList(options);
			}
			setOption(CURLOPT_TELNETOPTIONS, _slist_telnet_options.get());
		}

		void Curl::setNewFilePerms(long mode)
		{
			setOption(CURLOPT_NEW_FILE_PERMS, mode);
		}

		void Curl::setNewDirectoryPerms(long mode)
		{
			setOption(CURLOPT_NEW_DIRECTORY_PERMS, mode);
		}

		void Curl::setTFTPBlockSize(long size)
		{
			setOption(CURLOPT_TFTP_BLKSIZE, size);
		}

		void Curl::setTFTPNoOptions(bool v)
		{
#if defined(CURLOPT_TFTP_NO_OPTIONS)
			setOption(CURLOPT_TFTP_NO_OPTIONS, v);
#else
			throw std::runtime_error("Not supported by this CURL version");
#endif
		}

		void Curl::setRTSPRequest(RTSPRequest req)
		{
			long request = 0;
			switch (req)
			{
			case RTSPRequest::Announce: request = CURL_RTSPREQ_ANNOUNCE; break;
			case RTSPRequest::Describe: request = CURL_RTSPREQ_DESCRIBE; break;
			case RTSPRequest::GetParameter: request = CURL_RTSPREQ_GET_PARAMETER; break;
			case RTSPRequest::Options: request = CURL_RTSPREQ_OPTIONS; break;
			case RTSPRequest::Pause: request = CURL_RTSPREQ_PAUSE; break;
			case RTSPRequest::Play: request = CURL_RTSPREQ_PLAY; break;
			case RTSPRequest::Receive: request = CURL_RTSPREQ_RECEIVE; break;
			case RTSPRequest::Record: request = CURL_RTSPREQ_RECORD; break;
			case RTSPRequest::SetParameter: request = CURL_RTSPREQ_SET_PARAMETER; break;
			case RTSPRequest::Setup: request = CURL_RTSPREQ_SETUP; break;
			case RTSPRequest::Teardown: request = CURL_RTSPREQ_TEARDOWN; break;
			}
			setOption(CURLOPT_RTSP_REQUEST, request);
		}

		void Curl::setRTSPSessionID(const std::string & id)
		{
			setOption(CURLOPT_RTSP_SESSION_ID, id);
		}

		void Curl::setRTSPStreamURI(const std::string & uri)
		{
			setOption(CURLOPT_RTSP_STREAM_URI, uri);
		}

		void Curl::setRTSPTransport(const std::string & str)
		{
			setOption(CURLOPT_RTSP_TRANSPORT, str);
		}

		void Curl::setRTSPClientCSEQ(long seq)
		{
			setOption(CURLOPT_RTSP_CLIENT_CSEQ, seq);
		}

		void Curl::setRTSPServerCSEQ(long seq)
		{
			setOption(CURLOPT_RTSP_SERVER_CSEQ, seq);
		}

		void Curl::setSSHAuthTypes(const std::string & types)
		{
			long bitmask = 0;
			for (const std::string& str : stringSplit(" ", types))
			{
				if (str == "publickey") bitmask |= CURLSSH_AUTH_PUBLICKEY;
				else if (str == "password") bitmask |= CURLSSH_AUTH_PASSWORD;
				else if (str == "host") bitmask |= CURLSSH_AUTH_HOST;
				else if (str == "keyboard") bitmask |= CURLSSH_AUTH_KEYBOARD;
				else if (str == "agent") bitmask |= CURLSSH_AUTH_AGENT;
				else if (str == "any") bitmask |= CURLSSH_AUTH_ANY;
			}
			setOption(CURLOPT_SSH_AUTH_TYPES, bitmask);
		}

		void Curl::setSSHHostPublicKeyMD5(const std::string & md5)
		{
			setOption(CURLOPT_SSH_HOST_PUBLIC_KEY_MD5, md5);
		}

		void Curl::setSSHPublicKeyFile(const std::string & path)
		{
			setOption(CURLOPT_SSH_PUBLIC_KEYFILE, path);
		}

		void Curl::setSSHPrivateKeyFile(const std::string & path)
		{
			setOption(CURLOPT_SSH_PRIVATE_KEYFILE, path);
		}

		void Curl::setSSHKnownHostsFile(const std::string & path)
		{
			setOption(CURLOPT_SSH_KNOWNHOSTS, path);
		}

		void Curl::setSSHKeyFunction(std::function<SSHKeyStatus(const SSHKey&, const SSHKey&, SSHKeyMatch)> fn)
		{
			_ssh_key_match_fn = fn;
			setOption(CURLOPT_SSH_KEYFUNCTION, (void*)&_s_ssh_keycallback);
			setOption(CURLOPT_SSH_KEYDATA, this);
		}

		void Curl::setAutoReferer(bool v)
		{
			setOption(CURLOPT_AUTOREFERER, v);
		}

		void Curl::setAcceptEncoding(const std::string & str)
		{
			setOption(CURLOPT_ACCEPT_ENCODING, str);
		}

		void Curl::setTransferEncoding(const std::string & str)
		{
			setOption(CURLOPT_TRANSFER_ENCODING, str);
		}

		void Curl::setFollowLocation(bool v)
		{
			setOption(CURLOPT_FOLLOWLOCATION, v);
		}

		void Curl::setUnrestrictedAuth(bool v)
		{
			setOption(CURLOPT_UNRESTRICTED_AUTH, v);
		}

		void Curl::setMaxRedirections(long m)
		{
			setOption(CURLOPT_MAXREDIRS, m);
		}

		void Curl::setPostRedirection(const std::string & str)
		{
			long bitmask = 0;
			for (const std::string& s : stringSplit(" ", str))
			{
				if (s == "301") bitmask |= CURL_REDIR_POST_301;
				else if (s == "302") bitmask |= CURL_REDIR_POST_302;
				else if (s == "303") bitmask |= CURL_REDIR_POST_303;
				else if (s == "all") bitmask |= CURL_REDIR_POST_ALL;
			}
		}

		void Curl::setPOST(bool v)
		{
			setOption(CURLOPT_POST, v);
		}

		void Curl::setPOSTFields(const std::string & str)
		{
			setPOSTFieldSize(str.size());
			setOption(CURLOPT_COPYPOSTFIELDS, str);
		}

		void Curl::setPOSTFieldSize(size_t size)
		{
			curl_off_t csize = size;
			setOption(CURLOPT_POSTFIELDSIZE_LARGE, csize);
		}

		void Curl::setReferer(const std::string & referer)
		{
			setOption(CURLOPT_REFERER, referer);
		}

		void Curl::setUserAgent(const std::string & ua)
		{
			setOption(CURLOPT_USERAGENT, ua);
		}

		void Curl::setHeaders(const std::multimap<std::string, std::string>& headers)
		{
			std::vector<std::string> sheaders;
			for (auto elem : headers)
			{
				sheaders.push_back(elem.first + ": " + elem.second);
			}
			setOption(CURLOPT_HTTPHEADER, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_headers = toSList(sheaders);
			}
			setOption(CURLOPT_HTTPHEADER, _slist_headers.get());
		}

		void Curl::setHeaderOpt(bool v)
		{
			if (v) setOption(CURLOPT_HEADEROPT, (long)CURLHEADER_UNIFIED);
			else setOption(CURLOPT_HEADEROPT, (long)CURLHEADER_SEPARATE);
		}

		void Curl::setProxyHeaders(const std::multimap<std::string, std::string>& headers)
		{
			std::vector<std::string> sheaders;
			for (auto elem : headers)
			{
				sheaders.push_back(elem.first + ": " + elem.second);
			}
			setOption(CURLOPT_PROXYHEADER, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_proxyheaders = toSList(sheaders);
			}
			setOption(CURLOPT_PROXYHEADER, _slist_proxyheaders.get());
		}

		void Curl::setHTTP200Aliases(const std::vector<std::string>& aliases)
		{
			setOption(CURLOPT_HTTP200ALIASES, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_200_aliases = toSList(aliases);
			}
			setOption(CURLOPT_HTTP200ALIASES, _slist_200_aliases.get());
		}

		void Curl::setCookie(const std::string & cookie)
		{
			setOption(CURLOPT_COOKIE, cookie);
		}

		void Curl::setCookieFile(const std::string & cookiefile)
		{
			setOption(CURLOPT_COOKIEFILE, cookiefile);
		}

		void Curl::setCookieJar(const std::string & cookiejar)
		{
			setOption(CURLOPT_COOKIEJAR, cookiejar);
		}

		void Curl::setCookieSession(bool v)
		{
			setOption(CURLOPT_COOKIESESSION, v);
		}

		void Curl::setCookieList(const std::string & cookie)
		{
			setOption(CURLOPT_COOKIELIST, cookie);
		}

		void Curl::setHTTPGet(bool v)
		{
			setOption(CURLOPT_HTTPGET, v);
		}

		void Curl::setHTTPVersion(HTTPVersion v)
		{
			long version;
			switch (v)
			{
			case HTTPVersion::V1_0: version = CURL_HTTP_VERSION_1_0; break;
			case HTTPVersion::V1_1: version = CURL_HTTP_VERSION_1_1; break;
			case HTTPVersion::V2_0: version = CURL_HTTP_VERSION_2_0; break;
			case HTTPVersion::V2TLS: version = CURL_HTTP_VERSION_2TLS; break;
			default: version = CURL_HTTP_VERSION_NONE; break;
			}
			setOption(CURLOPT_HTTP_VERSION, version);
		}

		void Curl::setIgnoreContentLength(bool v)
		{
			setOption(CURLOPT_IGNORE_CONTENT_LENGTH, v);
		}

		void Curl::setContentDecoding(bool v)
		{
			setOption(CURLOPT_HTTP_CONTENT_DECODING, v);
		}

		void Curl::setTransferDecoding(bool v)
		{
			setOption(CURLOPT_HTTP_TRANSFER_DECODING, v);
		}

		void Curl::setExpect100TimeoutMs(long ms)
		{
			setOption(CURLOPT_EXPECT_100_TIMEOUT_MS, ms);
		}

		void Curl::setPipeWait(long wait)
		{
			setOption(CURLOPT_PIPEWAIT, wait);
		}

		void Curl::setStreamWeight(long w)
		{
			setOption(CURLOPT_STREAM_WEIGHT, w);
		}

		void Curl::setFTPPort(const std::string & spec)
		{
			setOption(CURLOPT_FTPPORT, spec);
		}

		void Curl::setQuote(const std::vector<std::string>& str)
		{
			setOption(CURLOPT_QUOTE, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_ftp_quote = toSList(str);
			}
			setOption(CURLOPT_QUOTE, _slist_ftp_quote.get());
		}

		void Curl::setPOSTQuote(const std::vector<std::string>& str)
		{
			setOption(CURLOPT_POSTQUOTE, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_ftp_postquote = toSList(str);
			}
			setOption(CURLOPT_POSTQUOTE, _slist_ftp_postquote.get());
		}

		void Curl::setPREQuote(const std::vector<std::string>& str)
		{
			setOption(CURLOPT_PREQUOTE, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_ftp_prequote = toSList(str);
			}
			setOption(CURLOPT_PREQUOTE, _slist_ftp_prequote.get());
		}

		void Curl::setFTPAppend(bool v)
		{
			setOption(CURLOPT_APPEND, v);
		}

		void Curl::setFTPUseEPRT(bool v)
		{
			setOption(CURLOPT_FTP_USE_EPRT, v);
		}

		void Curl::setFTPUseEPSV(bool v)
		{
			setOption(CURLOPT_FTP_USE_EPSV, v);
		}

		void Curl::setFTPUsePRET(bool v)
		{
			setOption(CURLOPT_FTP_USE_PRET, v);
		}

		void Curl::setFTPCreateDir(FTPCreateDir dir)
		{
			long option;
			switch (dir)
			{
			case FTPCreateDir::ALL: option = CURLFTP_CREATE_DIR; break;
			case FTPCreateDir::RETRY: option = CURLFTP_CREATE_DIR_RETRY; break;
			case FTPCreateDir::NONE:
			default: option = CURLFTP_CREATE_DIR_NONE; break;
			}
			setOption(CURLOPT_FTP_CREATE_MISSING_DIRS, option);
		}

		void Curl::setFTPResponseTimeout(long timeout)
		{
			setOption(CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
		}

		void Curl::setFTPAlternativeToUser(const std::string & alt)
		{
			setOption(CURLOPT_FTP_ALTERNATIVE_TO_USER, alt);
		}

		void Curl::setFTPSkipPASVIp(bool v)
		{
			setOption(CURLOPT_FTP_SKIP_PASV_IP, v);
		}

		void Curl::setFTPSSLAuth(FTPAuth auth)
		{
			long option;
			switch (auth)
			{
			case FTPAuth::TLS: option = CURLFTPAUTH_TLS; break;
			case FTPAuth::SSL: option = CURLFTPAUTH_SSL; break;
			case FTPAuth::DEFAULT:
			default: option = CURLFTPAUTH_DEFAULT; break;
			}
			setOption(CURLOPT_FTPSSLAUTH, option);
		}

		void Curl::setFTPSSLClearCommandChannel(FTPCCC ccc)
		{
			long option;
			switch (ccc)
			{
			case FTPCCC::PASSIVE: option = CURLFTPSSL_CCC_PASSIVE; break;
			case FTPCCC::ACTIVE: option = CURLFTPSSL_CCC_ACTIVE; break;
			case FTPCCC::NONE:
			default: option = CURLFTPSSL_CCC_NONE; break;
			}
			setOption(CURLOPT_FTP_SSL_CCC, option);
		}

		void Curl::setFTPAccount(const std::string & str)
		{
			setOption(CURLOPT_FTP_ACCOUNT, str);
		}

		void Curl::setFTPFileMethod(FTPFileMethod fm)
		{
			long option;
			switch (fm)
			{
			case FTPFileMethod::MULTICWD: option = CURLFTPMETHOD_MULTICWD; break;
			case FTPFileMethod::NOCWD: option = CURLFTPMETHOD_NOCWD; break;
			case FTPFileMethod::SINGLECWD: option = CURLFTPMETHOD_SINGLECWD; break;
			default: option = CURLFTPMETHOD_DEFAULT; break;
			}
			setOption(CURLOPT_FTP_FILEMETHOD, option);
		}

		void Curl::setTransferText(bool v)
		{
			setOption(CURLOPT_TRANSFERTEXT, v);
		}

		void Curl::setProxyTransferText(bool v)
		{
			setOption(CURLOPT_PROXY_TRANSFER_MODE, v);
		}

		void Curl::setConvertNewLine(bool v)
		{
			setOption(CURLOPT_CRLF, v);
		}

		void Curl::setRangeRequest(const std::string & str)
		{
			setOption(CURLOPT_RANGE, str);
		}

		void Curl::setResumeFrom(size_t s)
		{
			setOption(CURLOPT_RESUME_FROM_LARGE, (long)s);
		}

		void Curl::setCustomRequest(const std::string & req)
		{
			setOption(CURLOPT_CUSTOMREQUEST, req);
		}

		void Curl::setFileTime(bool v)
		{
			setOption(CURLOPT_FILETIME, v);
		}

		void Curl::setDirListOnly(bool v)
		{
			setOption(CURLOPT_DIRLISTONLY, v);
		}

		void Curl::setNoBody(bool v)
		{
			setOption(CURLOPT_NOBODY, v);
		}

		void Curl::setInFileSize(size_t s)
		{
			setOption(CURLOPT_INFILESIZE_LARGE, (long)s);
		}

		void Curl::setUpload(bool v)
		{
			setOption(CURLOPT_UPLOAD, v);
		}

		void Curl::setMaxFileSize(size_t s)
		{
			setOption(CURLOPT_MAXFILESIZE_LARGE, (long)s);
		}

		void Curl::setTimeCondition(TimeCondition tc)
		{
			long option;
			switch (tc)
			{
			case TimeCondition::MODSINCE: option = CURL_TIMECOND_IFMODSINCE; break;
			case TimeCondition::UNMODSINCE: option = CURL_TIMECOND_IFUNMODSINCE; break;
			case TimeCondition::LASTMOD: option = CURL_TIMECOND_LASTMOD; break;
			case TimeCondition::NONE:
			default: option = CURL_TIMECOND_NONE; break;
			}
			setOption(CURLOPT_TIMECONDITION, option);
		}

		void Curl::setTimeValue(long tv)
		{
			setOption(CURLOPT_TIMEVALUE, tv);
		}

		void Curl::setTimeout(long ms)
		{
			setOption(CURLOPT_TIMEOUT_MS, ms);
		}

		void Curl::setLowSpeedLimit(long bps)
		{
			setOption(CURLOPT_LOW_SPEED_LIMIT, bps);
		}

		void Curl::setLowSpeedTime(long s)
		{
			setOption(CURLOPT_LOW_SPEED_TIME, s);
		}

		void Curl::setMaxSendSpeed(long bps)
		{
			setOption(CURLOPT_MAX_SEND_SPEED_LARGE, bps);
		}

		void Curl::setMaxRecvSpeed(long bps)
		{
			setOption(CURLOPT_MAX_RECV_SPEED_LARGE, bps);
		}

		void Curl::setMaxConnections(long cnt)
		{
			setOption(CURLOPT_MAXCONNECTS, cnt);
		}

		void Curl::setFreshConnect(bool fresh)
		{
			setOption(CURLOPT_FRESH_CONNECT, fresh);
		}

		void Curl::setForbidReuse(bool close)
		{
			setOption(CURLOPT_FORBID_REUSE, close);
		}

		void Curl::setConnectTimeout(long timeoutms)
		{
			setOption(CURLOPT_CONNECTTIMEOUT_MS, timeoutms);
		}

		void Curl::setIPResolve(IPResolve res)
		{
			long option = CURL_IPRESOLVE_WHATEVER;
			switch (res)
			{
			case IPResolve::Whatever: option = CURL_IPRESOLVE_WHATEVER; break;
			case IPResolve::IPV4: option = CURL_IPRESOLVE_V4; break;
			case IPResolve::IPV6: option = CURL_IPRESOLVE_V6; break;
			}
			setOption(CURLOPT_IPRESOLVE, option);
		}

		void Curl::setConnectOnly(bool only)
		{
			setOption(CURLOPT_CONNECT_ONLY, only);
		}

		void Curl::setUseSSL(SSLLevel l)
		{
			long option;
			switch (l)
			{
			default:
			case SSLLevel::NONE: option = CURLUSESSL_NONE; break;
			case SSLLevel::OPT: option = CURLUSESSL_TRY; break;
			case SSLLevel::CONTROL: option = CURLUSESSL_CONTROL; break;
			case SSLLevel::ALL: option = CURLUSESSL_ALL; break;
			}
			setOption(CURLOPT_USE_SSL, option);
		}

		void Curl::setResolve(const std::vector<std::string>& list)
		{
			setOption(CURLOPT_RESOLVE, nullptr);
			{
				std::unique_lock<std::mutex> lck(_handle_lock);
				_slist_resolve = toSList(list);
			}
			setOption(CURLOPT_RESOLVE, _slist_resolve.get());
		}

		void Curl::setDNSInterface(const std::string& iface)
		{
			setOption(CURLOPT_DNS_INTERFACE, iface);
		}

		void Curl::setDNSLocalIPv4(const std::string& address)
		{
			setOption(CURLOPT_DNS_LOCAL_IP4, address);
		}

		void Curl::setDNSLocalIPv6(const std::string& address)
		{
			setOption(CURLOPT_DNS_LOCAL_IP6, address);
		}

		void Curl::setDNSServers(const std::string& servers)
		{
			setOption(CURLOPT_DNS_SERVERS, servers);
		}

		void Curl::setAcceptTimeout(long ms)
		{
			setOption(CURLOPT_ACCEPTTIMEOUT_MS, ms);
		}

		void Curl::setSSLCertificate(const std::string& file)
		{
			setOption(CURLOPT_SSLCERT, file);
		}

		void Curl::setSSLCertificateType(const std::string& type)
		{
			setOption(CURLOPT_SSLCERTTYPE, type);
		}

		void Curl::setSSLKey(const std::string& key)
		{
			setOption(CURLOPT_SSLKEY, key);
		}

		void Curl::setSSLKeyType(const std::string& type)
		{
			setOption(CURLOPT_SSLKEYTYPE, type);
		}

		void Curl::setSSLKeyPassword(const std::string& pass)
		{
			setOption(CURLOPT_KEYPASSWD, pass);
		}

		void Curl::setSSLEnableALPN(bool v)
		{
			setOption(CURLOPT_SSL_ENABLE_ALPN, v);
		}

		void Curl::setSSLEnableNPN(bool v)
		{
			setOption(CURLOPT_SSL_ENABLE_NPN, v);
		}

		void Curl::setSSLEngine(const std::string& str)
		{
			setOption(CURLOPT_SSLENGINE, str);
		}

		void Curl::setSSLEngineDefault(bool v)
		{
			setOption(CURLOPT_SSLENGINE_DEFAULT, v);
		}

		void Curl::setSSLFalseStart(bool v)
		{
			setOption(CURLOPT_SSL_FALSESTART, v);
		}

		void Curl::setSSLVersion(SSLVersion v)
		{
			setOption(CURLOPT_SSLVERSION, (long)v);
		}

		void Curl::setSSLVerifyHost(bool v)
		{
			setOption(CURLOPT_SSL_VERIFYHOST, (long)(v ? 2 : 0));
		}

		void Curl::setSSLVerifyPeer(bool v)
		{
			setOption(CURLOPT_SSL_VERIFYPEER, v);
		}

		void Curl::setSSLVerifyStatus(bool v)
		{
			setOption(CURLOPT_SSL_VERIFYSTATUS, v);
		}

		void Curl::setSSLCABundle(const std::string& path)
		{
			setOption(CURLOPT_CAINFO, path);
		}

		void Curl::setSSLIssuerCert(const std::string& path)
		{
			setOption(CURLOPT_ISSUERCERT, path);
		}

		void Curl::setSSLCAPath(const std::string& path)
		{
			setOption(CURLOPT_CAPATH, path);
		}

		void Curl::setSSLCRList(const std::string& file)
		{
			setOption(CURLOPT_CRLFILE, file);
		}

		void Curl::setSSLCertInfo(bool v)
		{
			setOption(CURLOPT_CERTINFO, v);
		}

		void Curl::setSSLPinnedPublicKey(const std::string& ppk)
		{
			setOption(CURLOPT_PINNEDPUBLICKEY, ppk);
		}

		void Curl::setSSLRandomFile(const std::string& file)
		{
			setOption(CURLOPT_RANDOM_FILE, file);
		}

		void Curl::setSSLEGDSocket(const std::string& socket)
		{
			setOption(CURLOPT_EGDSOCKET, socket);
		}

		void Curl::setSSLCipherList(const std::string& list)
		{
			setOption(CURLOPT_SSL_CIPHER_LIST, list);
		}

		void Curl::setSSLSessionIDCache(bool enabled)
		{
			setOption(CURLOPT_SSL_SESSIONID_CACHE, enabled);
		}

		void Curl::setSSLKRBLevel(const std::string& level)
		{
			setOption(CURLOPT_KRBLEVEL, level);
		}

		std::string Curl::getEffectiveURL()
		{
			std::string res;
			getInfo(CURLINFO_EFFECTIVE_URL, res);
			return res;
		}

		int Curl::getResponseCode()
		{
			long res;
			getInfo(CURLINFO_RESPONSE_CODE, res);
			return (int)res;
		}

		int Curl::getHTTPConnectCode()
		{
			long res;
			getInfo(CURLINFO_HTTP_CONNECTCODE, res);
			return (int)res;
		}

		long Curl::getFileTime()
		{
			long res;
			getInfo(CURLINFO_FILETIME, res);
			return res;
		}

		double Curl::getTotalTime()
		{
			double res;
			getInfo(CURLINFO_TOTAL_TIME, res);
			return res;
		}

		double Curl::getNameLookupTime()
		{
			double res;
			getInfo(CURLINFO_NAMELOOKUP_TIME, res);
			return res;
		}

		double Curl::getConnectTime()
		{
			double res;
			getInfo(CURLINFO_CONNECT_TIME, res);
			return res;
		}

		double Curl::getAppConnectTime()
		{
			double res;
			getInfo(CURLINFO_APPCONNECT_TIME, res);
			return res;
		}

		double Curl::getPreTransferTime()
		{
			double res;
			getInfo(CURLINFO_PRETRANSFER_TIME, res);
			return res;
		}

		double Curl::getStartTransferTime()
		{
			double res;
			getInfo(CURLINFO_STARTTRANSFER_TIME, res);
			return res;
		}

		double Curl::getRedirectTime()
		{
			double res;
			getInfo(CURLINFO_REDIRECT_TIME, res);
			return res;
		}

		long Curl::getRedirectCount()
		{
			long res;
			getInfo(CURLINFO_REDIRECT_COUNT, res);
			return res;
		}

		std::string Curl::getRedirectUrl()
		{
			std::string res;
			getInfo(CURLINFO_REDIRECT_URL, res);
			return res;
		}

		double Curl::getSizeUpload()
		{
			double res;
			getInfo(CURLINFO_SIZE_UPLOAD, res);
			return res;
		}

		double Curl::getSizeDownload()
		{
			double res;
			getInfo(CURLINFO_SIZE_DOWNLOAD, res);
			return res;
		}

		double Curl::getSpeedUpload()
		{
			double res;
			getInfo(CURLINFO_SPEED_UPLOAD, res);
			return res;
		}

		double Curl::getSpeedDownload()
		{
			double res;
			getInfo(CURLINFO_SPEED_DOWNLOAD, res);
			return res;
		}

		long Curl::getHeaderSize()
		{
			long res;
			getInfo(CURLINFO_HEADER_SIZE, res);
			return res;
		}

		long Curl::getRequestSize()
		{
			long res;
			getInfo(CURLINFO_REQUEST_SIZE, res);
			return res;
		}

		long Curl::getSSLVerifyResult()
		{
			long res;
			getInfo(CURLINFO_SSL_VERIFYRESULT, res);
			return res;
		}

		std::vector<std::string> Curl::getSSLEngines()
		{
			std::vector<std::string> res;
			getInfo(CURLINFO_SSL_ENGINES, res);
			return res;
		}

		double Curl::getContentLengthDownload()
		{
			double res;
			getInfo(CURLINFO_CONTENT_LENGTH_DOWNLOAD, res);
			return res;
		}

		double Curl::getContentLengthUpload()
		{
			double res;
			getInfo(CURLINFO_CONTENT_LENGTH_UPLOAD, res);
			return res;
		}

		std::string Curl::getContentType()
		{
			std::string res;
			getInfo(CURLINFO_CONTENT_TYPE, res);
			return res;
		}

		std::string Curl::getHTTPAuthAvailable()
		{
			std::string res;
			long info;
			getInfo(CURLINFO_HTTPAUTH_AVAIL, info);
			if (info & CURLAUTH_BASIC) res += "basic ";
			if (info & CURLAUTH_DIGEST) res += "digest ";
			if (info & CURLAUTH_DIGEST_IE) res += "digest_ie ";
			if (info & CURLAUTH_NEGOTIATE) res += "negotiate ";
			if (info & CURLAUTH_GSSNEGOTIATE) res += "gssnegotiate ";
			if (info & CURLAUTH_NTLM) res += "ntlm ";
			if (info & CURLAUTH_NTLM_WB) res += "ntlm_wb ";

			if (res != "" && res[res.size() - 1] == ' ')
				res.erase(res.begin() + res.size() - 1);

			return res;
		}

		std::string Curl::getProxyAuthAvailable()
		{
			std::string res;
			long info;
			getInfo(CURLINFO_PROXYAUTH_AVAIL, info);
			if (info & CURLAUTH_BASIC) res += "basic ";
			if (info & CURLAUTH_DIGEST) res += "digest ";
			if (info & CURLAUTH_DIGEST_IE) res += "digest_ie ";
			if (info & CURLAUTH_NEGOTIATE) res += "negotiate ";
			if (info & CURLAUTH_GSSNEGOTIATE) res += "gssnegotiate ";
			if (info & CURLAUTH_NTLM) res += "ntlm ";
			if (info & CURLAUTH_NTLM_WB) res += "ntlm_wb ";

			if (res != "" && res[res.size() - 1] == ' ')
				res.erase(res.begin() + res.size() - 1);

			return res;
		}

		long Curl::getOSErrorNumber()
		{
			long res;
			getInfo(CURLINFO_OS_ERRNO, res);
			return res;
		}

		long Curl::getNumConnects()
		{
			long res;
			getInfo(CURLINFO_NUM_CONNECTS, res);
			return res;
		}

		std::string Curl::getPrimaryIP()
		{
			std::string res;
			getInfo(CURLINFO_PRIMARY_IP, res);
			return res;
		}

		uint16_t Curl::getPrimaryPort()
		{
			long res;
			getInfo(CURLINFO_PRIMARY_PORT, res);
			return (uint16_t)res;
		}

		std::string Curl::getLocalIP()
		{
			std::string res;
			getInfo(CURLINFO_LOCAL_IP, res);
			return res;
		}

		uint16_t Curl::getLocalPort()
		{
			long res;
			getInfo(CURLINFO_LOCAL_PORT, res);
			return (uint16_t)res;
		}

		std::vector<std::string> Curl::getCookieList()
		{
			std::vector<std::string> res;
			getInfo(CURLINFO_COOKIELIST, res);
			return res;
		}

		uint64_t Curl::getActiveSocket()
		{
			curl_socket_t t;
			getInfo(CURLINFO_ACTIVESOCKET, (void**)&t);
			return t;
		}

		std::string Curl::getFTPEntryPath()
		{
			std::string res;
			getInfo(CURLINFO_FTP_ENTRY_PATH, res);
			return res;
		}

		std::vector<std::multimap<std::string, std::string>> Curl::getCertInfo()
		{
			std::vector<std::multimap<std::string, std::string>> res;
			struct curl_certinfo* info;
			getInfo(CURLINFO_CERTINFO, (void**)&info);
			for (int i = 0; i < info->num_of_certs; i++)
			{
				std::multimap<std::string, std::string> entry;
				struct curl_slist* list = info->certinfo[i];
				while (list != nullptr)
				{
					std::string line = list->data;
					auto pos = line.find_first_of(':');
					entry.insert({ line.substr(0,pos),line.substr(pos + 1) });
					list = list->next;
				}
				res.push_back(entry);
			}
			return res;
		}

		void Curl::setOutputString(std::string & str)
		{
			this->setWriteFunction([&str](auto data, auto len) {
				str += std::string(data, data + len);
				return len;
			});
		}

		void Curl::setInputString(const std::string & str)
		{
			this->setReadFunction([&str, pos = 0](auto data, auto len) mutable {
				std::string substr = str.substr(pos, (size_t)len);
				memcpy(data, substr.data(), substr.size());
				pos += substr.size();
				return substr.size();
			});
			this->setPOSTFieldSize(str.size());
		}

		void Curl::checkCode(int code)
		{
			if (code != CURLE_OK)
				throw std::runtime_error(
					"Curl returned error: " +
					std::string(curl_easy_strerror((CURLcode)code) +
						std::string(" (") +
						std::to_string(code) +
						std::string(")")));
		}

		size_t Curl::_s_write_callback(char * ptr, size_t size, size_t nmemb, void * userdata)
		{
			Curl* instance = (Curl*)userdata;
			if (!instance->_write_fn)
				return 0;
			return (size_t)instance->_write_fn(ptr, size*nmemb);
		}

		size_t Curl::_s_read_callback(char * ptr, size_t size, size_t nitems, void * userdata)
		{
			Curl* instance = (Curl*)userdata;
			if (!instance->_read_fn)
				return 0;
			return (size_t)instance->_read_fn(ptr, size*nitems);
		}

		int Curl::_s_ioctl_callback(void * handle, int cmd, void * userdata)
		{
			if (cmd != CURLIOCMD_RESTARTREAD)
				return CURLIOE_UNKNOWNCMD;
			Curl* instance = (Curl*)userdata;
			if (!instance->_ioctl_restart_fn)
				return CURLIOE_FAILRESTART;
			return instance->_ioctl_restart_fn() ? CURLIOE_OK : CURLIOE_FAILRESTART;
		}

		int Curl::_s_seek_callback(void * userdata, long long offset, int origin)
		{
			Curl* instance = (Curl*)userdata;
			try {
				return instance->_seek_fn(offset, origin) ? CURL_SEEKFUNC_OK : CURL_SEEKFUNC_CANTSEEK;
			}
			catch (std::exception) {
				return CURL_SEEKFUNC_FAIL;
			}
		}

		int Curl::_s_xferinfo_callback(void * userdata, long long dltotal, long long dlnow, long long ultotal, long long ulnow)
		{
			try {
				Curl* instance = (Curl*)userdata;
				if (instance->_xferinfo_fn) {
					instance->_xferinfo_fn(dltotal, dlnow, ultotal, ulnow);
				}
				return 0;
			}
			catch (std::exception)
			{
				return 1;
			}
		}

		size_t Curl::_s_header_callback(char * buffer, size_t size, size_t nitems, void * userdata)
		{
			try {
				Curl* instance = (Curl*)userdata;
				if (instance->_header_fn) {
					std::string str(buffer, buffer + (size*nitems));
					instance->_header_fn(str);
				}
				return size*nitems;
			}
			catch (std::exception)
			{
				return 0;
			}
		}

		int Curl::_s_debug_callback(void* handle, int type, char * data, size_t size, void *userdata)
		{
			Curl* instance = (Curl*)userdata;
			if (instance->_debug_fn)
				instance->_debug_fn((InfoType)type, data, size);
			return 0;
		}

		int Curl::_s_fnmatch_callback(void * ptr, const char * pattern, const char * string)
		{
			Curl* instance = (Curl*)ptr;
			try {
				return instance->_match_fn(pattern, string) ? CURL_FNMATCHFUNC_MATCH : CURL_FNMATCHFUNC_NOMATCH;
			}
			catch (std::exception)
			{
				return CURL_FNMATCHFUNC_FAIL;
			}
		}

		int Curl::_s_ssh_keycallback(void * curl, const void * knownkey, const void * foundkey, int khmatch, void * userdata)
		{
			Curl* instance = (Curl*)userdata;
			const struct curl_khkey* known = (const struct curl_khkey*)knownkey;
			const struct curl_khkey* found = (const struct curl_khkey*)foundkey;
			SSHKey key_known, key_found;
			key_known._data = known->len == 0 ? std::string(known->key) : std::string(known->key, known->key + known->len);
			key_found._data = found->len == 0 ? std::string(found->key) : std::string(found->key, found->key + found->len);
			key_known._type = (SSHKeyType)known->keytype;
			key_found._type = (SSHKeyType)found->keytype;

			return (int)(instance->_ssh_key_match_fn(key_known, key_found, (SSHKeyMatch)khmatch));
		}

		long Curl::getProtocolMask(const std::string & mask)
		{
			static std::map<std::string, long> names = {
				{ "http", CURLPROTO_HTTP },
				{ "https", CURLPROTO_HTTPS },
				{ "ftp", CURLPROTO_FTP },
				{ "ftps", CURLPROTO_FTPS },
				{ "scp", CURLPROTO_SCP },
				{ "sftp", CURLPROTO_SFTP },
				{ "telnet", CURLPROTO_TELNET },
				{ "ldap", CURLPROTO_LDAP },
				{ "ldaps", CURLPROTO_LDAPS },
				{ "dict", CURLPROTO_DICT },
				{ "file", CURLPROTO_FILE },
				{ "tftp", CURLPROTO_TFTP },
				{ "imap", CURLPROTO_IMAP },
				{ "imaps", CURLPROTO_IMAPS },
				{ "pop3", CURLPROTO_POP3 },
				{ "pop3s", CURLPROTO_POP3S },
				{ "smtp", CURLPROTO_SMTP },
				{ "smtps", CURLPROTO_SMTPS },
				{ "rtsp", CURLPROTO_RTSP },
				{ "rtmp", CURLPROTO_RTMP },
				{ "rtmpt", CURLPROTO_RTMPT },
				{ "rtmpe", CURLPROTO_RTMPE },
				{ "rtmpte", CURLPROTO_RTMPTE },
				{ "rtmps",CURLPROTO_RTMPS },
				{ "rtmpts", CURLPROTO_RTMPTS },
				{ "gopher", CURLPROTO_GOPHER },
				{ "smb", CURLPROTO_SMB },
				{ "smbs", CURLPROTO_SMBS },
				{ "all", CURLPROTO_ALL }
			};
			long result = 0;
			for (const std::string& name : stringSplit(" ", mask))
				result |= names[name];
			return result;
		}

		std::shared_ptr<void> Curl::toSList(const std::vector<std::string>& vector)
		{
			curl_slist* list = nullptr;
			for (const std::string& str : vector)
			{
				list = curl_slist_append(list, str.c_str());
			}
			return std::shared_ptr<void>(list, [](void* ptr) { curl_slist_free_all((curl_slist*)ptr); });
		}

		std::vector<std::string> Curl::fromSList(void * list)
		{
			struct curl_slist* slist = (struct curl_slist*)list;
			std::vector<std::string> res;
			while (slist != nullptr)
			{
				res.push_back(slist->data);
				slist = slist->next;
			}
			return res;
		}

		Curl::CurlGlobalInit Curl::_auto_init;

		Curl::CurlGlobalInit::CurlGlobalInit()
		{
			curl_global_init(CURL_GLOBAL_ALL);
		}

		Curl::CurlGlobalInit::~CurlGlobalInit()
		{
			curl_global_cleanup();
		}
	}
}
