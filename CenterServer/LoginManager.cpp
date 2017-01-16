#include "LoginManager.h"
#include "PacketProcessor_Center.h"
#include "PacketDispatcher.h"

#include "proto/server_server.pb.h"

using namespace S2SPacket;
using namespace TerraX;

LoginManager::LoginManager()
{
	REG_PACKET_HANDLER_ARG3(PktEnterPermissionReq, this, OnMessage_PktEnterPermissionReq);
}

void LoginManager::OnMessage_PktEnterPermissionReq(uint16_t channel_index, int32_t nGuestID, PktEnterPermissionReq* pkt)
{
	//check session key;

	//set guest info;

	PktEnterPermissionAck pktAck;
	pktAck.set_check_result(PktEnterPermissionAck_Result_ok);
	PacketProcessor_Center::GetInstance().SendPacket(channel_index, nGuestID, nGuestID, pktAck);

}

