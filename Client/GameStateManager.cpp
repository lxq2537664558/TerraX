#include <iostream>
#include "GameStateManager.h"
#include "PacketDispatcher.h"
#include "proto/client_server.pb.h"

using namespace TerraX;
using namespace C2SPacket;

GameStateManager::GameStateManager() : m_CurGameState(GameState_t::ePress2Start)
{
	m_GameStates[int(GameState_t::ePress2Start)].reset(new GameState_Press2Start);
	m_GameStates[int(GameState_t::eLoginForm)].reset(new GameState_eLoginForm);
	m_GameStates[int(GameState_t::eConnecting2Login)].reset(new GameState_Connecting2Login);
	m_GameStates[int(GameState_t::eAccountLoggingin)].reset(new GameState_AccountLoggingin);
	m_GameStates[int(GameState_t::eConnecting2Gate)].reset(new GameState_Connecting2Gate);
	m_GameStates[int(GameState_t::eAccountCheckingPermission)].reset(new GameState_CheckingPermission);
	m_GameStates[int(GameState_t::eAccountSelectingAvatar)].reset(new GameState_AccountSelectingAvatar);
	m_GameStates[int(GameState_t::eAccountEnteringWorld)].reset(new GameState_AccountEnteringWorld);
	m_GameStates[int(GameState_t::ePlayerEnteringScene)].reset(new GameState_PlayerEnteringScene);
	m_GameStates[int(GameState_t::eGaming)].reset(new GameState_Gaming);

	RegPacketHandler_Arg1(PktRoleListAck, std::bind(&GameStateManager::OnMessage_PktRoleListAck, this,
		std::placeholders::_1));
}

void GameStateManager::Tick() { m_GameStates[int(m_CurGameState)]->Tick(); }

void GameStateManager::NextState(GameState_t eGameState)
{
	m_GameStates[int(eGameState)]->Leave();
	m_CurGameState = eGameState;
	m_GameStates[int(m_CurGameState)]->Enter();
}

void GameStateManager::EnterDefaultState() { m_GameStates[int(GameState_t::ePress2Start)]->Enter(); }

void GameStateManager::OnMessage_PktRoleListAck(PktRoleListAck* pkt)
{
	assert(pkt);
	for (int i = 0; i < pkt->role_name_list_size(); ++i) {
		std::cout << pkt->role_name_list(i).c_str() << std::endl;
	}
	GameStateManager::GetInstance().NextState(GameState_t::eAccountSelectingAvatar);
}