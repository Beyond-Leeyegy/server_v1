/*
	
	FILE:network.cpp
	
	�����׽��ִ����Դ�����ļ�

*/

#include "def.h"
#include "client_structs.h"
#include "network.h"
#include "meta_data.h"
#include "linked_list.h"
#include "utils.h"
#include "data_protocal.h"

#include <string.h>
#include <stdio.h>


extern SOCKET g_SOCK;
extern sPCLIENT_DATA g_ClientList;
extern int g_TotalClient;

/*
	��ƽ̨��for win32
*/
#ifdef WIN32
void nonblock(SOCKET s) {
	u_long u10n = 1L;
	ioctlsocket(s, FIONBIO, (unsigned long *)&u10n);
}

#else
void nonblock(SOCKET s) {
	int flags;

	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
		log("* nonblock Error \r\n");
}

#endif

SOCKET InitServerSock(int port, int backlog) {
	struct  sockaddr_in sa;
	SOCKET sock = socket(AF_INET,SOCK_STREAM,0);

	if (sock < 0) {
		log(" InitServerSock(),socket(..)failed [PORT:%d]..\r\n", port);
		return -1;
	}

	/*
		���óɣ���ʹ��Ϊ�����������������Ҳ��������ͬ���ĵ�ַ����bind
	*/
#ifndef WIN32
	//���linuxƽ̨
	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
	{
		log("* Error: setsockopt REUSEADDR \r\n");
		exit(1);
	}
#endif // !WIN32

	/*
		LINGER���趨 ������
	*/
	//struct linger ld;
	//ld.l_onoff = 0;
	//ld.l_linger = 0;
	//if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ld, sizeof(ld)) < 0) {
		//log("* Error: setsockopt SO_LINGER...\r\n");
	//}


	/*
		�ṹ���ʼ��
	*/
	//memset((char*)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = INADDR_ANY;

	/*
		bind
	*/
	if (bind(sock, (const struct sockaddr *) &sa, sizeof(sa)) < 0) {
		log("InitServerSock(),bind(..) failed..[PORT:%d]\r\n", port);
		closesocket(sock);
		return -1;
	}

	/*
		����ΪNonblockingģʽ
	*/
	nonblock(sock);
	listen(sock, backlog);
	return sock;
	
}

/*
	�µ���������Ĵ���
	step1. accept()
	step2. �������ɵ��׽������ó�nonBlocking
	step3. ��ʼ��sPCLIENT_DATA 
	step4. �������ɵ�client���뵽�û������������ӵ��û���g_TotalClient
	step5. ������־
*/
void AcceptNewClient(SOCKET MotherSocket) {

	struct sockaddr_in peer;   //����¼�������ӵ��û���ip��ַ����Ϣ
	SOCKET newDesc;
	socklen_t peersize;


	peersize = sizeof(peer);

	newDesc = accept(MotherSocket,(struct sockaddr*)&peer,&peersize);


	/*
		Error: accpet����ֵ�쳣
	*/
	if (newDesc < 0) {
		log("[ERROR::ACCEPT_FAIL]Accept new client failed!\r\n\r\n\r\n");
		return;
	}

	nonblock(newDesc);

	sPCLIENT_DATA newClient = (sPCLIENT_DATA )malloc(sizeof(sCLIENT_DATA));

	/*
		Error:�ڴ����ʧ��
	*/
	if (!newClient) {
		log("[ERROR::memory allocating fail]\r\n\r\n\r\n");
		closesocket(newDesc);
		return;
	}

	/*
		��ʼ�����û�������
	*/
	newClient->m_scok = newDesc;

	/*

	for test
	*/
//	send(newDesc,"hello",strlen("hello"),0);


	strcpy(newClient->m_IP, (char *)inet_ntoa(peer.sin_addr));

	//recv data 
	*newClient->m_recvBuff = '\0'; //�ɼ�->������������*
	newClient->m_recvPos = 0;
	newClient->m_recvSize = 0;
	newClient->in_Game = 0;

	newClient->is_Ready = 0;

	//send data
	*newClient->m_sendBuff = '\0';
	newClient->m_sendSize = 0;


//	*newClient->m_sendBuff_syn = '\0';
//	newClient->m_sendSize_syn = 0;
//
//	*newClient->m_recvBuff_syn = '\0';
//	newClient->m_recvSize_syn  = 0;

	// set time
	newClient->m_lastRecvTime = timeGetTime();

	// set pointer
	newClient->m_prev = NULL;
	newClient->m_next = NULL;

	/*
		��������
	*/
	INSERT_TO_LIST(g_ClientList, newClient, m_prev, m_next); // ???m_next m_prev�����ã�����
	g_TotalClient++;

	/*
		������־
	*/
	log("*Accept New Connection: %d [%s] \r\n", newDesc, newClient->m_IP);

}

/*
�Ͽ�����
*/
void DisconnectClient(sPCLIENT_DATA pClient) {
	REMOVE_FROM_LIST(g_ClientList, pClient, m_prev, m_next);
	if (pClient->m_scok != INVALID_SOCKET)
		closesocket(pClient->m_scok);

	free(pClient);
	pClient = NULL;
}


/*
	��շ��ͻ�����������ĳ�ͻ������͵����ݷ��ͳ�ȥ�����������ȫ������Ҳ�п���ֻ������һ����
	ʹ�ó�����ĳ���׽����з��������ʱ�򣬵��ô˺���
*/
int FlushSendBuff(sPCLIENT_DATA pClient) {

	int sendSize;   //��¼�������ͳ�ȥ�����ݵĳ��ȣ���λ�ֽڣ�������-���ͻ���
	sendSize = send(pClient->m_scok, pClient->m_sendBuff, pClient->m_sendSize, 0);


	/*
		Error: caused by send function
	*/
	if (sendSize <= 0) {
		log("Error:client[%d] call send function but failed... in flushsendBuff", pClient->m_scok);
		return -1;
	}


	/*
		if some data sent failed
	*/
	if (sendSize < pClient->m_sendSize) {
		memmove(&pClient->m_sendBuff[0], &pClient->m_sendBuff[sendSize], pClient->m_sendSize - sendSize);
		pClient->m_sendSize -= sendSize;
	}
	else
	{
		*pClient->m_sendBuff = '\0';
		pClient->m_sendSize = 0;
	}


	log("[SEND::%d::%s]send:%d bytes left: %d bytes\r\n\r\n", pClient->m_scok, pClient->m_IP, sendSize, pClient->m_sendSize);

	return sendSize;
	

}

/*
	ĳ���׽�����read�������ʱ�򣬵��ô˺���
*/
BOOL RecvFromClient(sPCLIENT_DATA pClient) {
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv(pClient->m_scok, recvBuff, 1024, 0);
	if (recvSize == 0) {
		log("[Error reading::disconnct]:%s try to read socket while no data\r\n", pClient->m_IP);
		return false;
	}

	if (recvSize < 0) {

#if defined(Win32)
	/*
		WIN32
	*/
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			log("[Error handling::disconnct]:%s unknown error while read socket\r\n", pClient->m_IP);
			return false;

		}
#else
		/*
			LINUX
		*/
		if (errno != EWOULDBLOCK) {
			log("[Error handling::disconnct]:%s unknown error while read socket\r\n", pClient->m_IP);
			return false;
		}
#endif
		else
		{
			//pClient->m_scok
			return true; //�ٵĴ��󡪡���nonBlocking��������
		}
	}
	/*
		Buffer Overflow
	*/
	if ((pClient->m_recvSize + recvSize) >= dMAX_SOCK_BUFF) {
		log("[reading buffer overflow::disconnct]:%s try to read which resulting in overflow \r\n", pClient->m_IP);
		return false;
	}

	// record time 
	pClient->m_lastRecvTime = timeGetTime();

	memcpy(&pClient->m_recvBuff[pClient->m_recvSize], recvBuff, recvSize);
	pClient->m_recvSize += recvSize;

	return true;		// һ������
}
