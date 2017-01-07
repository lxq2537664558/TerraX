#pragma once
#include "ComDef.h"
#include <unordered_map>
#include <memory>
namespace S2SPacket
{
	class PktEnterPermissionReq;
}

namespace TerraX
{
	class CenterAccount{};
	class LoginManager
	{
		DISABLE_COPY(LoginManager);
		MAKE_INSTANCE(LoginManager);
	public:
		LoginManager();
		~LoginManager() = default;
	private:
		void OnMessage_PktEnterPermissionReq(uint16_t channel_index, int32_t nGuestID, S2SPacket::PktEnterPermissionReq* pkt);
		void OnMessage_PktCreateAccountReq(){}

	private:
		std::unordered_map<std::string, std::unique_ptr<CenterAccount>> m_TotalAccountInfos;
	};
}
