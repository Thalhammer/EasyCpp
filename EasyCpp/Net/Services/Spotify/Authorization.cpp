#include "Authorization.h"
#include "../../../StringAlgorithm.h"
#include "../../URI.h"
#include "../../Curl.h"
#include "../../../Base64.h"
#include "../../../Serialize/JsonSerializer.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Authorization::Authorization()
				{
				}

				Authorization::~Authorization()
				{
				}

				void Authorization::setClientID(const std::string & id)
				{
					_client_id = id;
				}

				const std::string & Authorization::getClientID()
				{
					return _client_id;
				}

				void Authorization::setClientSecret(const std::string & secret)
				{
					_client_secret = secret;
				}

				const std::string & Authorization::getClientSecret()
				{
					return _client_secret;
				}

				void Authorization::setRedirectURI(const std::string & uri)
				{
					_redirect_uri = uri;
				}

				const std::string & Authorization::getRedirectURI()
				{
					return _redirect_uri;
				}

				void Authorization::setScopes(const std::vector<std::string>& scopes)
				{
					_scopes = scopes;
				}

				const std::vector<std::string>& Authorization::getScopes()
				{
					return _scopes;
				}

				void Authorization::setShowDialog(bool sd)
				{
					_show_dialog = sd;
				}

				bool Authorization::getShowDialog()
				{
					return _show_dialog;
				}

				std::string Authorization::getAuthURL()
				{
					return "https://accounts.spotify.com/authorize?client_id=" + _client_id
						+ "&response_type=code&redirect_uri=" + URI::URLEncode(_redirect_uri)
						+ "&scope=" + URI::URLEncode(implode<std::string>(" ", _scopes))
						+ "&show_dialog=" + (_show_dialog ? "true" : "false");
				}

				AuthorizeResult Authorization::requestToken(const std::string & code)
				{
					std::string post = "grant_type=authorization_code&redirect_uri=" + URI::URLEncode(_redirect_uri) + "&code=" + code;
					std::string result;
					Curl c;
					c.setPOST(true);
					c.setPOSTFields(post);
					c.setURL("https://accounts.spotify.com/api/token");
					c.setSSLCABundle("curl-ca-bundle.crt");
					c.setOutputString(result);
					c.setHeaders({
						{ "Authorization", "Basic " + Base64::toString(_client_id + ":" + _client_secret) }
					});
					c.perform();

					AuthorizeResult res;
					res.fromAnyValue(Serialize::JsonSerializer().deserialize(result));
					return res;
				}

				AuthorizeResult Authorization::refreshToken(const std::string & refresh_token)
				{
					std::string post = "grant_type=refresh_token&refresh_token=" + refresh_token;
					std::string result;
					Curl c;
					c.setPOST(true);
					c.setPOSTFields(post);
					c.setURL("https://accounts.spotify.com/api/token");
					c.setSSLCABundle("curl-ca-bundle.crt");
					c.setOutputString(result);
					c.setHeaders({
						{ "Authorization", "Basic " + Base64::toString(_client_id + ":" + _client_secret) }
					});
					c.perform();

					AuthorizeResult res;
					res.fromAnyValue(Serialize::JsonSerializer().deserialize(result));
					if (res.getRefreshToken() == "")
						res._refresh_token = refresh_token;
					return res;
				}

				AuthorizeResult Authorization::requestToken()
				{
					std::string post = "grant_type=client_credentials";
					std::string result;
					Curl c;
					c.setPOST(true);
					c.setPOSTFields(post);
					c.setURL("https://accounts.spotify.com/api/token");
					c.setSSLCABundle("curl-ca-bundle.crt");
					c.setOutputString(result);
					c.setHeaders({
						{ "Authorization", "Basic " + Base64::toString(_client_id + ":" + _client_secret) }
					});
					c.perform();

					AuthorizeResult res;
					res.fromAnyValue(Serialize::JsonSerializer().deserialize(result));
					return res;
				}
			}
		}
	}
}