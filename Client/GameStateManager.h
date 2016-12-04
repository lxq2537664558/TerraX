#pragma once
#include "ComDef.h"
#include "GameState.h"
#include <array>
#include <memory>
namespace TerraX
{
	using GameStateArray = std::array<std::unique_ptr<IGameState>, int32_t(GameState_t::eGameState_Count)>;
	class GameStateManager final
	{
		NOCOPY(GameStateManager);
		MAKEINSTANCE(GameStateManager);
	public:
		GameStateManager();
		~GameStateManager() = default;
		void Tick();
		void EnterDefaultState();
		void NextState(GameState_t eGameState);
	private:
		GameState_t m_CurGameState{ GameState_t::eUnKnownState };
		GameStateArray m_GameStates;
	};
}
