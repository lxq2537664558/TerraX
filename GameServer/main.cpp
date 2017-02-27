#include <iostream>
#include <stdio.h>
#include <google/protobuf/message.h>
#include "GameServer.h"

#ifdef _WIN32
#include <WinSock2.h>
#endif

using namespace google::protobuf;
using namespace TerraX;

int main_asdf(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	if (GameServer::GetInstance().Init())
	{
		GameServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();
#ifdef _WIN32
	WSACleanup();
#endif
	getchar();
	return 0;
}

#include "Avatar.h"
#include "AvatarComponent.h"
#include "MissionComponent.h"
#include "base_guid.hpp"
int main()
{
	std::unique_ptr<Avatar> pAvatar(new Avatar());
	pAvatar->InitComponents();
	pAvatar->GetAvatarComponent()->SetExp(1200);

	Container_T<64> mc;
	mc.Push(new MissionItem(nullptr, nullptr));
	mc.Push(new MissionItem(nullptr, nullptr));
	auto iter = mc.Iterator();
	while (!iter.IsEnd())
	{
		auto pItem = iter.Next();
		static int nCount = 0;
		std::cout << ++nCount << std::endl;
	}
	std::cin.get();
	return 0;
}