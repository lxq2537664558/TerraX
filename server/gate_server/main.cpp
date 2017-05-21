#include "gate_server/gate_server.h"

using namespace terra;

int main(int argc, char* argv[])
{
	if (GateServer::GetInstance().Init())
	{
		GateServer::GetInstance().AfterInit();
		GateServer::GetInstance().Execute();
		GateServer::GetInstance().BeforeShut();
		GateServer::GetInstance().Shut();
	}
}