#include "UDPServer.h"
#include <stdio.h>


UDPServer::UDPServer()
{


}


UDPServer::~UDPServer()
{

}


void UDPServer::SetCallback(void *ptr)
{
    server_call_back = (SERVER_CALLBACK)ptr;
}


DWORD WINAPI ServerThread (LPVOID parms)
{
  int len;
  UDPServer *srv = (UDPServer*)parms;
  char recv_buf[2048];
  int addr_len = sizeof(srv->addr);

  while(srv->server_active)
  {
    len  = recvfrom(srv->sock,
                    recv_buf,
                    2048,
                    0,
                    (SOCKADDR *)&(srv->addr),
                    &addr_len
                   );

    if(len<=0)
        break;

    if(len > 0)
       srv->server_call_back(recv_buf, len);

   }

   return 0;
}

int UDPServer::Open(int port)
{
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_active = 0;
    result = bind(sock, (SOCKADDR *) &addr, sizeof(addr));
    if(result == 0)
    {
       hServerThread = CreateThread(NULL,
                                    0,
                                    ServerThread,
                                    this, 0,
                                    &ServerThreadID);

    }

    if (hServerThread == INVALID_HANDLE_VALUE)
    {
		result = -1;
	}

	if(sock == INVALID_SOCKET)
    {
        result = -1;
    }

    if(result == 0)
    {
       server_active = 1;
    }

    return result;
}


int UDPServer::Send(char *buf, int len)
{
   int result = sendto(sock,
                       buf,
                       len,
                       0,
                       (SOCKADDR *) &addr,
                       sizeof(addr));

   return result;
}


int UDPServer::Close(void)
{
    server_active = 0;
    int result = closesocket(sock);
    WSACleanup();
    return result;
}



