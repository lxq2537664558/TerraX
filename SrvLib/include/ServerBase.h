#pragma once

namespace TerraX
{
	class ServerBase
	{
	public:
		virtual bool Init();
		virtual void Run();
		virtual void Exit();

		void StopMainLoop() { m_bExitLoop = true;  }

	private:
		bool m_bExitLoop{ false };
	};
}