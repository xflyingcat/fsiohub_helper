#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

#include <windows.h>
#include <stdio.h>
#include "winsock2.h"

typedef int (*SERVER_CALLBACK)(char *buf, int len);

class UDPServer
{
  public:
   SOCKET sock;
   struct sockaddr_in addr;
   struct sockaddr_in addr_by_id;
   int server_active;
   HANDLE hServerThread;
   DWORD ServerThreadID;
   SERVER_CALLBACK server_call_back;
   UDPServer();
   ~UDPServer();
   int Open(int port);
   void SetCallback(void *ptr);
   int Close(void);
   int Send(char *buf, int len);

  private:
    WSADATA wsaData;
};




#endif


