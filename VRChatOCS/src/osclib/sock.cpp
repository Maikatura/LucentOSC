#include "sock.h"

#ifdef _WIN32
#include <WinSock2.h>
void sock::Startup()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
}
void sock::Cleanup()
{
	WSACleanup();
}
#else
void sock::Startup()
{
}
void sock::Cleanup()
{
}
#endif
