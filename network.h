#pragma once
/*
	FILE:network.h

	�׽��ִ�������ͷ�ļ�

*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "def.h"
#include "client_structs.h"


void nonblock(SOCKET s);
SOCKET InitServerSock(int port, int backlog);
void AcceptNewClient(SOCKET MotherSocket);
void DisconnectClient(sPCLIENT_DATA pClient);

//send
int FlushSendBuff(sPCLIENT_DATA pClient);

//recv
BOOL RecvFromClient(sPCLIENT_DATA pClient);

#endif // !__NETWORK_H__

