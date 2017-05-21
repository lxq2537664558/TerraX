#include "server_login_ack_service.h"

#include "comm/net/packet_dispatcher.h"
#include "packet_processor.h"

using namespace terra;
using namespace packet_ss;

ServerLoginAckService::ServerLoginAckService(ServerAcceptService& accept_service, PeerType_t peer, int max_conns)
	: accept_service_(accept_service),
	kSelfPeer(peer),
	max_conns_(max_conns)
{
    for (int i = 1; i < max_conns_; i++) {
        server_ids_.push(i);
    }
    ServerTable::GetInstance().SetServerTableEventCB(
        [this](const std::vector<Net_Object>& objs, const Net_Object& net_obj,
               ServerTableEvent_t ev) { this->OnServerTableEvent(objs, net_obj, ev); });

    REG_PACKET_HANDLER_ARG3(MsgRegisterSW, this, OnMessage_RegisterSW);
}
ServerLoginAckService::~ServerLoginAckService() {}

void ServerLoginAckService::OnLoginOut(TcpConnection* conn)
{
    Net_Object* net_object = ServerTable::GetInstance().GetNetObjectByConn(conn);
    assert(net_object);
    server_ids_.push(net_object->server_id_);
}

void ServerLoginAckService::OnServerTableEvent(const std::vector<Net_Object>& objs, const Net_Object& net_obj, ServerTableEvent_t ev)
{
    if (net_obj.peer_type_ == PeerType_t::GATESERVER) {
        MsgServerInfoWS msg;
        msg.set_event(int(ev));
        for (const auto& obj : objs) {
            if (obj.peer_type_ == PeerType_t::NODESERVER) {
                const auto& srv_info = msg.add_server_info();
                srv_info->set_peer_type(int(obj.peer_type_));
                srv_info->set_server_id(obj.server_id_);
                srv_info->set_listen_ip(obj.listen_ip_);
                srv_info->set_listen_port(obj.listen_port_);
            }
        }
        //PacketProcessor::GetInstance().SendPacket(net_obj.conn_, msg);
    } else if (net_obj.peer_type_ == PeerType_t::NODESERVER) {
        for (const auto& obj : objs) {
            if (obj.peer_type_ == PeerType_t::GATESERVER) {
				MsgServerInfoWS msg;
                msg.set_event(int(ev));
                const auto& srv_info = msg.add_server_info();
                srv_info->set_peer_type(int(net_obj.peer_type_)); //该node的ip发送给其他gate
                srv_info->set_server_id(net_obj.server_id_);
                srv_info->set_listen_ip(net_obj.listen_ip_);
                srv_info->set_listen_port(net_obj.listen_port_);
                //PacketProcessor::GetInstance().SendPacket(obj.conn_, msg);
            }
        }
    } else {
    }
}

void ServerLoginAckService::OnMessage_RegisterSW(TcpConnection* conn, int32_t avatar_id, MsgRegisterSW* msg)
{
    assert(server_ids_.size() > 0);

    int server_id = server_ids_.front();
    server_ids_.pop();
    int peer_type = msg->peer_type();

    LOG_INFO(LEVEL_INFO, "%s:\t %d \n", NetHelper::ServerName(PeerType_t(peer_type)), server_id);

	MsgRegisterWS msgWS;
	msgWS.set_server_id(server_id);
	PacketProcessor::GetInstance().SendPacket(conn, msgWS);

    ServerTable::GetInstance().AddServerInfo(static_cast<PeerType_t>(peer_type), server_id, msg->listen_ip().c_str(), msg->listen_port(), conn);
}