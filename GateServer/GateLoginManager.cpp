#include "GateLoginManager.h"
#include "PacketDispatcher.h"
#include "Guest.h"
#include "GuestManager.h"
#include "PacketProcessor_Gate.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"

using namespace TerraX;
using namespace C2SPacket;
using namespace S2SPacket;

GateLoginManager::GateLoginManager()
{
	RegPacketHandlerFunction();
}

void GateLoginManager::RegPacketHandlerFunction()
{
	REG_PACKET_HANDLER_ARG2(PktGameLoginReq, std::bind(&GateLoginManager::OnMessage_PktGameLoginReq, this, std::placeholders::_1, std::placeholders::_2));
	REG_PACKET_HANDLER_ARG2(PktEnterPermissionAck, std::bind(&GateLoginManager::OnMessage_PktEnterPermissionAck, this, std::placeholders::_1, std::placeholders::_2));
}

void GateLoginManager::OnMessage_PktGameLoginReq(int guest_id, PktGameLoginReq* pkt)
{
	auto pGuest = GuestManager::GetInstance().GetGuest(guest_id);
	if (!pGuest)
	{
		assert(false);
		return;
	}
	pGuest->InitAccountInfo(new GateAccount(pkt->account_name()));

	PktEnterPermissionReq pktReq;
	pktReq.set_account_name(pkt->account_name());
	pktReq.set_session_key(pkt->session_key());
	PacketProcessor_Gate::GetInstance().SendPacket2Server(pGuest->GetDestPeerInfo(PeerType_t::centerserver), guest_id, pktReq);
}


void GateLoginManager::OnMessage_PktEnterPermissionAck(int guest_id, PktEnterPermissionAck* pkt)
{
	auto pGuest = GuestManager::GetInstance().GetGuest(guest_id);
	if (!pGuest)
	{
		assert(false);
		return;
	}
	//request avatar details on redis db
	PktRoleListAck pktAck;
	auto p1 = pktAck.add_role_name_list();
	*p1 = "Dolores";
	auto p2 = pktAck.add_role_name_list();
	*p2 = "Teddy";
	auto p3 = pktAck.add_role_name_list();
	*p3 = "Arnold";
	PacketProcessor_Gate::GetInstance().SendPacket2Client(PeerInfo::GetChannelIndex(guest_id), guest_id, pktAck);
}