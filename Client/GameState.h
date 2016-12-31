#pragma once

namespace TerraX
{
	enum class GameState_t
	{
		eUnKnownState = -1,
		ePress2Start = 0,				// 启动界面
		eLoginForm,						// 登录界面	
		eConnecting2Login,				// 正在连接登录服务器...弹出窗口
		eAccountLoggingin,				// 正在登录中...弹窗,收到成功包则进入服务器选择界面或者申请进入游戏世界,失败则返回登录阶段
		/*eAccountChosingServer,*/		// 选择服务器界面
		eAcc,
		eAccountEnteringWorld,			// 进入游戏世界中...成功则进入角色选择界面，失败返回登录界面
		eAccountSelectingAvatar,		// 选择界面 //（选择，创建，删除等等是否需要新增状态？）
		ePlayerEnteringScene,			// 进入场景
		eGaming,						// 游戏中

		eGameState_Count,
	};

	class IGameState
	{
	public:
		virtual void Enter() = 0;
		virtual void Tick() = 0;
		virtual void Leave() = 0;
	};

	class GameState_Press2Start : public IGameState
	{
	public:
		void Enter() override final;
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_eLoginForm : public IGameState
	{
	public:
		void Enter() override final;
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_Connecting2Login : public IGameState
	{
	public:
		void Enter() override final {}
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_AccountLoggingin : public IGameState
	{
	public:
		void Enter() override final {}
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_Connecting2Gate : public IGameState
	{
	public:
		void Enter() override final;
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_AccountEnteringWorld : public IGameState
	{
	public:
		void Enter() override final;
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_AccountSelectingAvatar : public IGameState
	{
	public:
		void Enter() override final {}
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_PlayerEnteringScene : public IGameState
	{
	public:
		void Enter() override final {}
		void Tick() override final {}
		void Leave() override final {}
	};

	class GameState_Gaming : public IGameState
	{
	public:
		void Enter() override final {}
		void Tick() override final {}
		void Leave() override final {}
	};

}
