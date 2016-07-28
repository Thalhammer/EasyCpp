#pragma once
#include "../DllExport.h"
#include "../NonCopyable.h"
#include <mutex>
#include <map>
#include <vector>
#include "URI.h"
#include <memory>

namespace EasyCpp
{
	namespace Net
	{
		typedef int CurlOption;
		typedef int CurlInfo;
		class DLL_EXPORT Curl : public NonCopyable
		{
		public:
			enum class InfoType
			{
				Text = 0,
				HeaderIn,
				HeaderOut,
				DataIn,
				DataOut,
				SSLDataIn,
				SSLDataOut
			};
			enum class ProxyType
			{
				HTTP,
				HTTP_1_0,
				SOCKS4,
				SOCKS5,
				SOCKS4A,
				SOCKS5_HOSTNAME
			};
			enum class RTSPRequest
			{
				Options,
				Describe,
				Announce,
				Setup,
				Play,
				Pause,
				Teardown,
				GetParameter,
				SetParameter,
				Record,
				Receive
			};
			enum class SSHKeyStatus
			{
				FineAddToFile,
				Fine,
				Reject,
				Defer
			};
			enum class SSHKeyMatch
			{
				OK,
				Mismatch,
				Missing
			};
			enum class SSHKeyType
			{
				UNKNOWN,
				RSA1,
				RSA,
				DSS
			};
			class SSHKey
			{
			public:
				const std::string& getKeyData();
				SSHKeyType getType();
			private:
				std::string _data;
				SSHKeyType _type;
				friend class Curl;
			};
			enum class HTTPVersion
			{
				NONE, // We don't care what curl uses
				V1_0,
				V1_1,
				V2_0,
				V2TLS
			};
			enum class FTPCreateDir
			{
				NONE,
				ALL,
				RETRY
			};
			enum class FTPAuth
			{
				DEFAULT,
				SSL,
				TLS
			};
			enum class FTPCCC
			{
				NONE,
				PASSIVE,
				ACTIVE
			};
			enum class FTPFileMethod
			{
				MULTICWD,
				NOCWD,
				SINGLECWD
			};
			enum class TimeCondition
			{
				MODSINCE,
				UNMODSINCE,
				LASTMOD,
				NONE
			};
			enum class IPResolve
			{
				Whatever,
				IPV4,
				IPV6
			};
			enum class SSLLevel
			{
				NONE,
				OPT,
				CONTROL,
				ALL
			};
			enum class SSLVersion
			{
				DEFAULT,
				TLSv1,
				SSLv2,
				SSLv3,
				TLSv1_0,
				TLSv1_1,
				TLSv1_2
			};

			Curl();
			virtual ~Curl();

			void perform();
			bool receive(void* buffer, size_t buflen, size_t& bytes_read);
			bool send(void* buffer, size_t buflen, size_t& bytes_send);
			void reset();

			/* Behavior Options*/
			void setVerbose(bool v);
			void setHeader(bool v);
			void setNoProgress(bool v);
			void setWildcardMatch(bool v);

			/* Network Options*/
			void setURL(const std::string& url);
			void setURL(const URI& uri);
			void setPathAsIs(bool v);
			void setProtocols(const std::string& protocols);
			void setRedirectProtocols(const std::string& protocols);
			void setDefaultProtocol(const std::string& protocol);
			void setProxy(const std::string& proxy);
			void setProxyPort(uint16_t port);
			void setProxyType(ProxyType type);
			void setNoProxy(const std::string& hosts);
			void setHTTPProxyTunnel(bool v);
			void setConnectTo(const std::vector<std::string>& list);
			void setSocks5GSSAPINegotiationProtection(bool v);
			void setProxyServiceName(const std::string& service);
			void setAuthenticationServiceName(const std::string& name);
			void setInterface(const std::string& str);
			void setLocalPort(uint16_t port);
			void setLocalPortRange(uint16_t range);
			void setDNSCacheTimeout(long timeout);
			void setBufferSize(long size);
			void setPort(uint16_t port);
			void setTCPFastOpen(bool v);
			void setTCPNoDelay(bool v);
			void setAddressScope(long scope);
			void setTCPKeepAlive(bool v);
			void setTCPKeepAliveIdleTime(long time);
			void setTCPKeepAliveInterval(long time);
			void setUnixSocketPath(const std::string& path);

			/* Names and Password Options */
			void setNETRC(bool read, bool required);
			void setNETRCFilename(const std::string& name);
			void setUsername(const std::string& user);
			void setPassword(const std::string& pass);
			void setProxyUsername(const std::string& user);
			void setProxyPassword(const std::string& password);
			void setLoginOptions(const std::string& options);
			void setHTTPAuth(const std::string& methods);
			void setTLSAuthUsername(const std::string& user);
			void setTLSAuthPassword(const std::string& pass);
			void setTLSAuthType(const std::string& type);
			void setProxyAuth(const std::string& methods);
			void setSASLIR(bool v);
			void setXOAuth2Bearer(const std::string& token);

			/* Callback Options */
			void setWriteFunction(std::function<uint64_t(char*, uint64_t)> fn);
			void setReadFunction(std::function<uint64_t(char*, uint64_t)> fn);
			void setStreamRestartFunction(std::function<bool()> fn);
			void setSeekFunction(std::function<bool(uint64_t, int)> fn);
			void setXferInfoFunction(std::function<void(uint64_t, uint64_t, uint64_t, uint64_t)> fn);
			void setHeaderFunction(std::function<void(std::string)> fn);
			void setDebugFunction(std::function<void(InfoType, char*, size_t)> fn);
			void setMatchFunction(std::function<bool(const std::string& pattern, const std::string& string)> fn);

			void setDebugString(std::string& debug);

			/* Error Options */
			void setFailOnError(bool v);
			std::string getErrorBuffer() const;

			/* SMTP Options */
			void setMailFrom(const std::string& from);
			void setMailRecipients(const std::vector<std::string>& rcpt);
			void setMailAuth(const std::string& auth);

			/* TELNET Options */
			void setTelnetOptions(const std::vector<std::string>& options);

			/* Other Options */
			void setNewFilePerms(long mode);
			void setNewDirectoryPerms(long mode);

			/* TFTP Options */
			void setTFTPBlockSize(long size);
			void setTFTPNoOptions(bool v);

			/* RTSP Options */
			void setRTSPRequest(RTSPRequest req);
			void setRTSPSessionID(const std::string& id);
			void setRTSPStreamURI(const std::string& uri);
			void setRTSPTransport(const std::string& str);
			void setRTSPClientCSEQ(long seq);
			void setRTSPServerCSEQ(long seq);

			/* SSH Options */
			void setSSHAuthTypes(const std::string& types);
			void setSSHHostPublicKeyMD5(const std::string& md5);
			void setSSHPublicKeyFile(const std::string& path);
			void setSSHPrivateKeyFile(const std::string& path);
			void setSSHKnownHostsFile(const std::string& path);
			void setSSHKeyFunction(std::function<SSHKeyStatus(const SSHKey&, const SSHKey&, SSHKeyMatch)> fn);

			/* HTTP Options */
			void setAutoReferer(bool v);
			void setAcceptEncoding(const std::string& str);
			void setTransferEncoding(const std::string& str);
			void setFollowLocation(bool v);
			void setUnrestrictedAuth(bool v);
			void setMaxRedirections(long m);
			void setPostRedirection(const std::string& str);
			void setPOST(bool v);
			void setPOSTFields(const std::string& str);
			void setPOSTFieldSize(size_t size);
			void setReferer(const std::string& referer);
			void setUserAgent(const std::string& ua);
			void setHeaders(const std::multimap<std::string, std::string>& headers);
			void setHeaderOpt(bool v);
			void setProxyHeaders(const std::multimap<std::string, std::string>& headers);
			void setHTTP200Aliases(const std::vector<std::string>& aliases);
			void setCookie(const std::string& cookie);
			void setCookieFile(const std::string& cookiefile);
			void setCookieJar(const std::string& cookiejar);
			void setCookieSession(bool v);
			void setCookieList(const std::string& cookie);
			void setHTTPGet(bool v);
			void setHTTPVersion(HTTPVersion v);
			void setIgnoreContentLength(bool v);
			void setContentDecoding(bool v);
			void setTransferDecoding(bool v);
			void setExpect100TimeoutMs(long ms);
			void setPipeWait(long wait);
			void setStreamWeight(long w);

			/* FTP Options */
			void setFTPPort(const std::string& spec);
			void setQuote(const std::vector<std::string>& str);
			void setPOSTQuote(const std::vector<std::string>& str);
			void setPREQuote(const std::vector<std::string>& str);
			void setFTPAppend(bool v);
			void setFTPUseEPRT(bool v);
			void setFTPUseEPSV(bool v);
			void setFTPUsePRET(bool v);
			void setFTPCreateDir(FTPCreateDir dir);
			void setFTPResponseTimeout(long timeout);
			void setFTPAlternativeToUser(const std::string& alt);
			void setFTPSkipPASVIp(bool v);
			void setFTPSSLAuth(FTPAuth auth);
			void setFTPSSLClearCommandChannel(FTPCCC ccc);
			void setFTPAccount(const std::string& str);
			void setFTPFileMethod(FTPFileMethod fm);

			/* Protocol Options */
			void setTransferText(bool v);
			void setProxyTransferText(bool v);
			void setConvertNewLine(bool v);
			void setRangeRequest(const std::string& str);
			void setResumeFrom(size_t s);
			void setCustomRequest(const std::string& req);
			void setFileTime(bool v);
			void setDirListOnly(bool v);
			void setNoBody(bool v);
			void setInFileSize(size_t s);
			void setUpload(bool v);
			void setMaxFileSize(size_t s);
			void setTimeCondition(TimeCondition tc);
			void setTimeValue(long tv);

			/* Connection Options */
			void setTimeout(long ms);
			void setLowSpeedLimit(long bps);
			void setLowSpeedTime(long s);
			void setMaxSendSpeed(long bps);
			void setMaxRecvSpeed(long bps);
			void setMaxConnections(long cnt);
			void setFreshConnect(bool fresh);
			void setForbidReuse(bool close);
			void setConnectTimeout(long timeoutms);
			void setIPResolve(IPResolve res);
			void setConnectOnly(bool only);
			void setUseSSL(SSLLevel l);
			void setResolve(const std::vector<std::string>& list);
			void setDNSInterface(const std::string& iface);
			void setDNSLocalIPv4(const std::string& address);
			void setDNSLocalIPv6(const std::string& address);
			void setDNSServers(const std::string& servers);
			void setAcceptTimeout(long ms);

			/* SSL/Security Options */
			void setSSLCertificate(const std::string& file);
			void setSSLCertificateType(const std::string& type);
			void setSSLKey(const std::string& key);
			void setSSLKeyType(const std::string& type);
			void setSSLKeyPassword(const std::string& pass);
			void setSSLEnableALPN(bool v);
			void setSSLEnableNPN(bool v);
			void setSSLEngine(const std::string& str);
			void setSSLEngineDefault(bool v);
			void setSSLFalseStart(bool v);
			void setSSLVersion(SSLVersion v);
			void setSSLVerifyHost(bool v);
			void setSSLVerifyPeer(bool v);
			void setSSLVerifyStatus(bool v);
			void setSSLCABundle(const std::string& path);
			void setSSLIssuerCert(const std::string& path);
			void setSSLCAPath(const std::string& path);
			void setSSLCRList(const std::string& file);
			void setSSLCertInfo(bool v);
			void setSSLPinnedPublicKey(const std::string& ppk);
			void setSSLRandomFile(const std::string& file);
			void setSSLEGDSocket(const std::string& socket);
			void setSSLCipherList(const std::string& list);
			void setSSLSessionIDCache(bool enabled);
			void setSSLKRBLevel(const std::string& level);

			/*
			*	TODO:
			*	- CURLOPT_HTTPPOST
			*	- CURLOPT_STREAM_DEPENDS
			*	- CURLOPT_STREAM_DEPENDS_E
			*	- CURLOPT_SSL_OPTIONS
			*	- CURLOPT_GSSAPI_DELEGATION
			*/

			std::string getEffectiveURL();
			int getResponseCode();
			int getHTTPConnectCode();
			long getFileTime();
			double getTotalTime();
			double getNameLookupTime();
			double getConnectTime();
			double getAppConnectTime();
			double getPreTransferTime();
			double getStartTransferTime();
			double getRedirectTime();
			long getRedirectCount();
			std::string getRedirectUrl();
			double getSizeUpload();
			double getSizeDownload();
			double getSpeedUpload();
			double getSpeedDownload();
			long getHeaderSize();
			long getRequestSize();
			long getSSLVerifyResult();
			std::vector<std::string> getSSLEngines();
			double getContentLengthDownload();
			double getContentLengthUpload();
			std::string getContentType();
			std::string getHTTPAuthAvailable();
			std::string getProxyAuthAvailable();
			long getOSErrorNumber();
			long getNumConnects();
			std::string getPrimaryIP();
			uint16_t getPrimaryPort();
			std::string getLocalIP();
			uint16_t getLocalPort();
			std::vector<std::string> getCookieList();
			long getLastSocket();
			std::string getFTPEntryPath();
			std::vector<std::multimap<std::string, std::string>> getCertInfo();

			/*
			*	TODO:
			*	CURLINFO_TLS_SSL_PTR
			*	CURLINFO_CONDITION_UNMET
			*	CURLINFO_RTSP_SESSION_ID
			*	CURLINFO_RTSP_CLIENT_CSEQ
			*	CURLINFO_RTSP_SERVER_CSEQ
			*	CURLINFO_RTSP_CSEQ_RECV
			*/

			/* Helper shortcuts */
			void setOutputString(std::string& str);
			void setInputString(const std::string& str);

		private:
			void* _handle;
			std::mutex _handle_lock;
			char* _error_buffer;

			std::shared_ptr<void> _slist_mail_rcpt;
			std::shared_ptr<void> _slist_telnet_options;
			std::shared_ptr<void> _slist_connect_to;
			std::shared_ptr<void> _slist_headers;
			std::shared_ptr<void> _slist_proxyheaders;
			std::shared_ptr<void> _slist_200_aliases;
			std::shared_ptr<void> _slist_ftp_quote;
			std::shared_ptr<void> _slist_ftp_postquote;
			std::shared_ptr<void> _slist_ftp_prequote;
			std::shared_ptr<void> _slist_resolve;

			std::function<uint64_t(char*, uint64_t)> _write_fn;
			std::function<uint64_t(char*, uint64_t)> _read_fn;
			std::function<bool()> _ioctl_restart_fn;
			std::function<bool(uint64_t, int)> _seek_fn;
			std::function<void(uint64_t, uint64_t, uint64_t, uint64_t)> _xferinfo_fn;
			std::function<void(std::string)> _header_fn;
			std::function<void(InfoType, char*, size_t)> _debug_fn;
			std::function<bool(const std::string& pattern, const std::string& string)> _match_fn;
			std::function<SSHKeyStatus(const SSHKey&, const SSHKey&, SSHKeyMatch)> _ssh_key_match_fn;

			// Set Curl Option
			void setOption(CurlOption option, void* val);
			void setOption(CurlOption option, bool val);
			void setOption(CurlOption option, long val);
			void setOption(CurlOption option, long long val);
			void setOption(CurlOption option, const std::string& val);
			// Get Curl Info
			void getInfo(CurlInfo info, long& val);
			void getInfo(CurlInfo info, std::string& val);
			void getInfo(CurlInfo info, std::vector<std::string>& val);
			void getInfo(CurlInfo info, double& val);
			void getInfo(CurlInfo info, void** val);
			static void checkCode(int code);

			static size_t _s_write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
			static size_t _s_read_callback(char* ptr, size_t size, size_t nitems, void* userdata);
			static int _s_ioctl_callback(void* handle, int cmd, void* userdata);
			static int _s_seek_callback(void* userdata, long long offset, int origin);
			static int _s_xferinfo_callback(void* userdata, long long dltotal, long long dlnow, long long ultotal, long long ulnow);
			static size_t _s_header_callback(char *buffer, size_t size, size_t nitems, void *userdata);
			static int _s_debug_callback(void* handle, int type, char *data, size_t size, void *userdata);
			static int _s_fnmatch_callback(void *ptr, const char *pattern, const char *string);
			static int _s_ssh_keycallback(void* curl, const void* knownkey, const void* foundkey, int curl_khmatch, void *userdata);

			static long getProtocolMask(const std::string& name);

			static std::shared_ptr<void> toSList(const std::vector<std::string>& vector);
			static std::vector<std::string> fromSList(void* slist);

			class CurlGlobalInit
			{
			public:
				CurlGlobalInit();
				~CurlGlobalInit();
			};
			static CurlGlobalInit _auto_init;
		};
	}
}
