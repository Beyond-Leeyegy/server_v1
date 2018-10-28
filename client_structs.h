#pragma once
#ifndef __CLIENT_STRUCTS_H__
#define __CLIENT_STRUCTS_H__
#include "def.h"
#include "read_write_on_bit.h"

#define dMAX_SOCK_BUFF 1024	// buffer length
#define dNAME_LEN 50		// name length 
#define dIP_LEN 20			// IP length


struct client_data
{
	/*
		DATA
	*/

	//client->server
	char m_recvBuff[dMAX_SOCK_BUFF];	
	int  m_recvSize;					//����������ݰ��ܳ���
	int  m_recvPos;     

	//server->client
	char m_sendBuff[dMAX_SOCK_BUFF];	
	int  m_sendSize;					//�����͵����ݰ��ܳ���

//	char m_recvBuff_syn[dMAX_SOCK_BUFF];
//	char m_sendBuff_syn[dMAX_SOCK_BUFF];
//	int m_recvSize_syn;
//	int m_sendSize_syn;



	/*
		CLIENT STATE
	*/

	int in_Game = 0; 
	int is_Ready = 0;
	int player_type;    //pClient1 : 0 , pClient2: 1



	/*
		NETWORK
	*/

	// network communication
	int m_scok;			// ���������׽���
	char m_IP[dIP_LEN];     // IP  address

	//���һ������ʱ�䣬���ڲ����Ƿ����
	unsigned long m_lastRecvTime;



	/*
		LIST
	*/
	struct client_data *m_prev;
	struct client_data *m_next;

};


typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;




#endif
