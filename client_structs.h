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
	int  m_recvSize;					//待处理的数据包总长度
	int  m_recvPos;     

	//server->client
	char m_sendBuff[dMAX_SOCK_BUFF];	
	int  m_sendSize;					//待发送的数据包总长度

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
	int m_scok;			// 网络连接套接字
	char m_IP[dIP_LEN];     // IP  address

	//最后一次连接时间，用于测试是否掉线
	unsigned long m_lastRecvTime;



	/*
		LIST
	*/
	struct client_data *m_prev;
	struct client_data *m_next;

};


typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;




#endif
