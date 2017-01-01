#include "LoginManager.h"
#include "PacketProcessor_Center.h"
#include "PacketDispatcher.h"

#include "proto/server_server.pb.h"

using namespace S2SPacket;
using namespace TerraX;

LoginManager::LoginManager()
{
	RegPacketHandler_Arg3(PktRegisterReq,
		std::bind(&LoginManager::OnMessage_PktEnterPermissionReq, this, std::placeholders::_1,
			std::placeholders::_2, std::placeholders::_3));
}

void LoginManager::OnMessage_PktEnterPermissionReq(int32_t nChannelInfo, int32_t nGuestID, PktEnterPermissionReq* pkt)
{
	//check session key;

	//set guest info;

	PktEnterPermissionAck pktAck;
	pktAck.set_check_result(PktEnterPermissionAck_Result_ok);
	PacketProcessor_Center::GetInstance().SendPacket(nChannelInfo, nGuestID, nGuestID, pktAck);
}

