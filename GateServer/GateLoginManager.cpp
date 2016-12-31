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
	Guest* pGuest = GuestManager::GetInstance().GetGuest(guest_id);
	if (!pGuest)
	{
		assert(false);
		return;
	}
	//pGuest->

	PktEnterPermissionReq pktReq;
	pktReq.set_account_name(pkt->account_name());
	pktReq.set_session_key(pkt->session_key());
	int nDestInfo = pGuest->GetDestPeerInfo(PeerType_t::centerserver);
	PacketProcessor_Gate::GetInstance().SendPacket2BackEnd(nDestInfo, guest_id, pktReq);
}