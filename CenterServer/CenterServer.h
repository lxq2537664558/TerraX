#pragma once
#include "ComDef.h"
#include "IServer.h"

namespace TerraX
{
    const int MAX_CONNECTION = 64;
    class CenterServer final : public IServer
    {
        NOCOPY(CenterServer);
        MAKEINSTANCE(CenterServer);

    public:
        CenterServer() = default;
        ~CenterServer() = default;

        bool Init(/*Config Info*/) override;
        void Run() override;
        void Exit() override { m_bExit = true; }

    protected:
        bool InitStaticModule() override;
        bool InitNetModule() override;
        void ProcessLogic() override;

    private:
        bool m_bExit{false};
    };
}