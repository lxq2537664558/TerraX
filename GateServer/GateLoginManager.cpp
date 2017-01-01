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
	RegPacketHandler_Arg2(PktGameLoginReq, std::bind(&GateLoginManager::OnMessage_PktGameLoginReq, this, std::placeholders::_1, std::placeholders::_2));
}

void GateLoginManager::OnMessage_PktGameLoginReq(int guest_id, PktGameLoginReq* pkt)
{
	Guest* pGuest = GuestManager::GetInstance().CreateGuest(guest_id);
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