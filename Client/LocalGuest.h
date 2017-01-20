#pragma once
#include "base_macro.h"
#include <string>

namespace TerraX
{
	class LocalGuest
	{
		DISABLE_COPY(LocalGuest);
		MAKE_INSTANCE(LocalGuest);

	public:
		LocalGuest() = default;
		~LocalGuest() = default;

		void InitLoginInfo(std::string& account, std::string& password) {
			m_AccountName = std::move(account);
			m_Password = std::move(password);
			//Encrypt
		}

		std::string& GetAccountName() { return m_AccountName; }
		std::string& GetSessionKey() { return m_SessionKey; }

	private:
		void EncryptPwd(std::string& password) {};
		void DecryptPwd(std::string& password) {};
	private:
		std::string m_AccountName;
		std::string m_Password;
		std::string m_SessionKey;
	};
}