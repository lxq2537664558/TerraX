#include "GameState.h"
#include <iostream>
#include <stdio.h>
#include "GameStateManager.h"
#include "proto/client_server.pb.h"
#include "PacketSender.h"
using namespace TerraX;
using namespace C2SPacket;

void GameState_Press2Start::Enter()
{
	std::cout << "Press any key to continue..." << std::endl;
	getchar();
	GameStateManager::GetInstance().NextState(GameState_t::eLoginForm);
}


void GameState_eLoginForm::Enter()
{
	std::cout << "Please input you account:\t " << std::endl;
	char szAccountName[32] = { 0 };
	std::cin >> szAccountName;
	//std::cout << "Please input you password:\t " << std::endl;
	//char szPassword[32] = { 0 };
	//std::cin >> szPassword;
	//GameStateManager::GetInstance().NextState(GameState_t::eConnecting2Login);

	GameStateManager::GetInstance().NextState(GameState_t::eAccountEnteringWorld);
}

void GameState_AccountEnteringWorld::Enter()
{
	PktAccountRequestEnterWorld pkt;
	pkt.set_szaccountname("ghost");
	pkt.set_szsessionkey("key session");
	//SendPacket2World();
	//PacketSender::GetInstance().SendPacket();
}