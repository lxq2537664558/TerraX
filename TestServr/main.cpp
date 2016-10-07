#include <iostream>
#include "ComDef.h"
#include "ServerBase.h"
using namespace TerraX;

class TestServer : public ServerBase
{
	CreateInstance(TestServer);
public:
	TestServer() = default;
	~TestServer() = default;

	//bool Init() override { return true; };
	//void Run() override {}
};

int main(int argc, char* argv[])
{
	if (TestServer::GetInstance().Init())
	{
		TestServer::GetInstance().Run();
	}
	std::cout << "Test Server!" << std::endl;
	getchar();
	return 0;
}