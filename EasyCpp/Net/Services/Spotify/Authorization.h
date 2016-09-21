#pragma once
#include "../../../DllExport.h"
#include <string>
#include <vector>
#include "AuthorizeResult.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Authorization
				{
				public:
					Authorization();
					~Authorization();
					void setClientID(const std::string& id);
					const std::string& getClientID();
					void setClientSecret(const std::string& secret);
					const std::string& getClientSecret();
					void setRedirectURI(const std::string& uri);
					const std::string& getRedirectURI();
					void setScopes(const std::vector<std::string>& scopes);
					const std::vector<std::string>& getScopes();
					void setShowDialog(bool sd);
					bool getShowDialog();

					std::string getAuthURL();

					AuthorizeResult requestToken(const std::string& code);
					AuthorizeResult refreshToken(const std::string& refresh_token);

				private:
					std::string _client_id;
					std::string _client_secret;
					std::string _redirect_uri;
					std::vector<std::string> _scopes;
					bool _show_dialog;
				};
			}
		}
	}
}