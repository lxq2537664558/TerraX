#include "GameStateManager.h"

using namespace TerraX;

GameStateManager::GameStateManager() : m_CurGameState(GameState_t::ePress2Start)
{
	m_GameStates[int(GameState_t::ePress2Start)].reset(new GameState_Press2Start);
	m_GameStates[int(GameState_t::eLoginForm)].reset(new GameState_eLoginForm);
	m_GameStates[int(GameState_t::eConnecting2Login)].reset(new GameState_Connecting2Login);
	m_GameStates[int(GameState_t::eAccountLoggingin)].reset(new GameState_AccountLoggingin);
	m_GameStates[int(GameState_t::eAccountEnteringWorld)].reset(new GameState_AccountEnteringWorld);
	m_GameStates[int(GameState_t::eAccountSelectingAvatar)].reset(new GameState_AccountSelectingAvatar);
	m_GameStates[int(GameState_t::ePlayerEnteringScene)].reset(new GameState_PlayerEnteringScene);
	m_GameStates[int(GameState_t::eGaming)].reset(new GameState_Gaming);

	m_GameStates[int(m_CurGameState)]->Enter();
}

void GameStateManager::Tick()
{
	m_GameStates[int(m_CurGameState)]->Tick();
}

void GameStateManager::NextState(GameState_t eGameState)
{
	m_GameStates[int(eGameState)]->Leave();
	m_CurGameState = eGameState;
	m_GameStates[int(m_CurGameState)]->Enter();
}