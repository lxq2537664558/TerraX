#include "GameState.h"
#include <iostream>
#include <stdio.h>
#include "GameStateManager.h"
#include "NetManagerClient.h"
#include "LocalGuest.h"
#include "proto/client_server.pb.h"

using namespace C2SPacket;
using namespace TerraX;

void GameState_Press2Start::Enter()
{
	std::cout << "Press Enter to continue...";
	getchar();
	GameStateManager::GetInstance().NextState(GameState_t::eLoginForm);
}


void GameState_eLoginForm::Enter()
{
	std::cout << "Please input you account:\t " << std::endl;
	std::string strAccountName;
	std::getline(std::cin, strAccountName);
	std::cout << "Please input you password:\t " << std::endl;
	std::string strPassword;
	std::getline(std::cin, strPassword);
	LocalGuest::GetInstance().InitLoginInfo(strAccountName, strPassword);
	//GameStateManager::GetInstance().NextState(GameState_t::eConnecting2Login);

	GameStateManager::GetInstance().NextState(GameState_t::eConnecting2Gate);
}

void GameState_Connecting2Gate::Enter()
{
	NetManagerClient::GetInstance().Connect("127.0.0.1", 9991);
}

void GameState_CheckingPermission::Enter()
{
	PktGameLoginReq pkt;
	pkt.set_account_name(LocalGuest::GetInstance().GetAccountName());
	pkt.set_session_key(LocalGuest::GetInstance().GetSessionKey());
	NetManagerClient::GetInstance().SendPacket(PeerType_t::gateserver, pkt);
}

void GameState_AccountEnteringWorld::Enter()
{
}