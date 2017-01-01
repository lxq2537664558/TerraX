#pragma once
#include <string>

namespace TerraX
{
	class GateAccount
	{
	public:
		GateAccount(const std::string& strAccountName) : m_AccountName(strAccountName){}
	private:
		std::string m_AccountName;
	};
}