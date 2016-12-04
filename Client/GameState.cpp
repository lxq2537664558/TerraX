#include "GameState.h"
#include <iostream>
#include <stdio.h>
#include "GameStateManager.h"
#include "NetManagerClient.h"
using namespace TerraX;

void GameState_Press2Start::Enter()
{
	//std::cout << "Press any key to continue..." << std::endl;
	//getchar();
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
	NetManagerClient::GetInstance().Connect("127.0.0.1", 9991, true);
}