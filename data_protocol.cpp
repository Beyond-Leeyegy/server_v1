#include "data_protocal.h"
#include "meta_data.h"
#include "utils.h"
#include "game.hpp"


//�����ǿͻ��˻��Ƿ������ˣ����ǽ����ݴ浽��Ӧclient��д������
/*
�������ˣ�д��Ҫ�����ķ�������д������
�ͻ��ˣ���ʱֻ��һ��Client�ṹ�壬���Ҳ��д������ṹ���д������������IP�Ƿ�����IP
*/
void onCache_PLAYER_MOVE(sPCLIENT_DATA toClient) {
	//���ݰ��в���Ҫ�������ݣ�������Ҫһ��������ԭ����Ϊ�˷��������͸�ָ�����׽���
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	//��װ����
	//Tag
	PutByte(packet, PLAYER_MOVE, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (toClient->m_sendSize + pos > dMAX_SOCK_BUFF) {
		log("[WARNING::OVERFLOW]client [%d] onCache_PLAYER_MOVE\r\n", toClient->m_scok);
	}
	else {
		memcpy(&toClient->m_sendBuff[toClient->m_sendSize], packet, pos);
		toClient->m_sendSize += pos;
		log("[CACHE::onCache_PLAYER_MOVE] [%d] [%s] onCache_PLAYER_MOVE\r\n\r\n", toClient->m_scok, toClient->m_IP);
	}
}

void onCache_SET_PLAYER_DIR(sPCLIENT_DATA toClient, int dir) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)dir;

	if (type != dir) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_SET_PLAYER_DIR\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, SET_PLAYER_DIR, pos);

	//Player_type
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (toClient->m_sendSize + pos > dMAX_SOCK_BUFF) {
		log("[WARNING::OVERFLOW]client [%d] onCache_SET_PLAYER_DIR\r\n", toClient->m_scok);
	}
	else {
		memcpy(&toClient->m_sendBuff[toClient->m_sendSize], packet, pos);
		toClient->m_sendSize += pos;
		log("[CACHE::onCache_SET_PLAYER_DIR] [%d] [%s] onCache_SET_PLAYER_DIR\r\n\r\n", toClient->m_scok, toClient->m_IP);
	}

}
void onCache_CREATE_PLAYER_BULLET(sPCLIENT_DATA toClient) {
	//���ݰ��в���Ҫ�������ݣ�������Ҫһ��������ԭ����Ϊ�˷��������͸�ָ�����׽���
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	//��װ����
	//Tag
	PutByte(packet, CREATE_PLAYER_BULLET, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (toClient->m_sendSize + pos > dMAX_SOCK_BUFF) {
		log("[WARNING::OVERFLOW]client [%d] onCache_CREATE_PLAYER_BULLET\r\n", toClient->m_scok);
	}
	else {
		memcpy(&toClient->m_sendBuff[toClient->m_sendSize], packet, pos);
		toClient->m_sendSize += pos;
		log("[CACHE::onCache_CREATE_PLAYER_BULLET] [%d] [%s] onCache_CREATE_PLAYER_BULLETr\n\r\n", toClient->m_scok, toClient->m_IP);
	}
}

void onCache_dPACKET_IS_READY(sPCLIENT_DATA pClient) {
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;


	//��װ����
	//Tag
	PutByte(packet, dPACKET_IS_READY, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient->m_sendSize + pos > dMAX_SOCK_BUFF) {
		log("[WARNING::OVERFLOW]client [%d] onCache_dPACKET_IS_READY\r\n", pClient->m_scok);
	}
	else {
		memcpy(&pClient->m_sendBuff[pClient->m_sendSize], packet, pos);
		pClient->m_sendSize += pos;
		log("[CACHE::onCache_dPACKET_IS_READY] [%d] [%s] onCache_dPACKET_IS_READYr\n\r\n", pClient->m_scok, pClient->m_IP);
	}
}

void onCache_KILL_PLAYER_J(int player_type, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)player_type;

	if (type != player_type) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_KILL_PLAYER_J\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, KILL_PLAYER_J, pos);

	//Player_type
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_KILL_PLAYER_J\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s] onCache_KILL_PLAYER_Jr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_KILL_PLAYER_J\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s]onCache_KILL_PLAYER_J r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}


void onCache_REMOVE_ENEMY_BULLET(int i, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {//ɾ����i���ӵ�
																						 //���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD index = (WORD)i;

	if (index != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD IN onCache_REMOVE_ENEMY_BULLET\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, REMOVE_ENEMY_BULLET, pos);

	//Player_index
	PutWord(packet, index, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]\r\n in onCache_REMOVE_ENEMY_BULLET", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s]in onCache_REMOVE_ENEMY_BULLET r\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]in onCache_REMOVE_ENEMY_BULLET\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s]in onCache_REMOVE_ENEMY_BULLET r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}

}


void onCache_CREATE_BONUS(int bonusType, int row, int col, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)bonusType;
	WORD _row = (WORD)row;
	WORD _col = (WORD)col;


	if (type != bonusType || _row != row || col != _col) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_CREATE_BONUS\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, CREATE_BONUS, pos);

	PutWord(packet, type, pos);
	PutWord(packet, row, pos);
	PutWord(packet, col, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_CREATE_BONUS\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s] onCache_CREATE_BONUS\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_CREATE_BONUS\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_KILL_PLAYER_J] [%d] [%s]onCache_CREATE_BONUS r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}


void onCache_KILL_ENEMY_K(int i, int k, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD i_ = (WORD)i;
	WORD type = (WORD)k;

	if (type != k || i_ != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_KILL_ENEMY_K\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, KILL_ENEMY_K, pos);

	//k
	PutWord(packet, i_, pos);
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_KILL_ENEMY_K\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_KILL_ENEMY_K] [%d] [%s] onCache_KILL_ENEMY_Kr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_KILL_ENEMY_K\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_KILL_ENEMY_K] [%d] [%s]onCache_KILL_ENEMY_K r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

void onCache_REMOVE_PLAYER_BULLET_I_J(int i, int j, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;
	WORD _j = (WORD)j;

	if (type != i || _j != j) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_REMOVE_PLAYER_BULLET_I_J\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, REMOVE_PLAYER_BULLET_I_J, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_REMOVE_PLAYER_BULLET_I_J\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_REMOVE_PLAYER_BULLET_I_J] [%d] [%s] onCache_REMOVE_PLAYER_BULLET_I_Jr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_REMOVE_PLAYER_BULLET_I_J\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_REMOVE_PLAYER_BULLET_I_J] [%d] [%s]onCache_REMOVE_PLAYER_BULLET_I_J r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}
void onCache_PLAYER_BULLET_HIT_PLAYER(int i, int j, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;
	WORD _j = (WORD)j;

	if (type != i || _j != j) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_PLAYER_BULLET_HIT_PLAYER\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, PLAYER_BULLET_HIT_PLAYER, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_PLAYER_BULLET_HIT_PLAYER\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_PLAYER] [%d] [%s] onCache_PLAYER_BULLET_HIT_PLAYERr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_PLAYER_BULLET_HIT_PLAYER\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_PLAYER] [%d] [%s]onCache_PLAYER_BULLET_HIT_PLAYER r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}

}
void onCache_PLAYER_BULLET_HIT_PLAYER_BULLET(int i, int j, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;
	WORD _j = (WORD)j;

	if (type != i || _j != j) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_PLAYER_BULLET_HIT_PLAYER_BULLET\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, PLAYER_BULLET_HIT_PLAYER_BULLET, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_PLAYER_BULLET_HIT_PLAYER_BULLET\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_PLAYER_BULLET] [%d] [%s] onCache_PLAYER_BULLET_HIT_PLAYER_BULLETr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_PLAYER_BULLET_HIT_PLAYER_BULLET\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_PLAYER_BULLET] [%d] [%s]onCache_PLAYER_BULLET_HIT_PLAYER_BULLET r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}

}
void onCache_PLAYER_BULLET_HIT_ENEMY_BULLET(int i, int j, int k, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;
	WORD _j = (WORD)j;
	WORD _k = (WORD)k;

	if (type != i || _j != j || _k != k) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_PLAYER_BULLET_HIT_ENEMY_BULLET\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, PLAYER_BULLET_HIT_ENEMY_BULLET, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutWord(packet, _k, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_PLAYER_BULLET_HIT_ENEMY_BULLET\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_ENEMY_BULLET] [%d] [%s] onCache_PLAYER_BULLET_HIT_ENEMY_BULLETr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_PLAYER_BULLET_HIT_ENEMY_BULLET\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_ENEMY_BULLET] [%d] [%s]onCache_PLAYER_BULLET_HIT_ENEMY_BULLET r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}
void onCache_PLAYER_BULLET_HIT_FENCE(int i, int j, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;
	WORD _j = (WORD)j;

	if (type != i || _j != j) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_PLAYER_BULLET_HIT_FENCE\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, PLAYER_BULLET_HIT_FENCE, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_PLAYER_BULLET_HIT_FENCE\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_FENCE] [%d] [%s] onCache_PLAYER_BULLET_HIT_FENCEr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_PLAYER_BULLET_HIT_FENCE\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_PLAYER_BULLET_HIT_FENCE] [%d] [%s]onCache_PLAYER_BULLET_HIT_FENCE r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}
void onCache_ALL_PLAYER_BULLET_MOVE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;


	//��װ����
	//Tag
	PutByte(packet, ALL_PLAYER_BULLET_MOVE, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_ALL_PLAYER_BULLET_MOVE\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_ALL_PLAYER_BULLET_MOVE] [%d] [%s] onCache_ALL_PLAYER_BULLET_MOVEr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_ALL_PLAYER_BULLET_MOVE\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_ALL_PLAYER_BULLET_MOVE] [%d] [%s]onCache_ALL_PLAYER_BULLET_MOVE r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}
void onCache_CREATE_ENEMY_BORE(int i, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;

	if (type != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_CREATE_ENEMY_BORE\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, CREATE_ENEMY_BORE, pos);

	//k
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_CREATE_ENEMY_BORE\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_CREATE_ENEMY_BORE] [%d] [%s] onCache_CREATE_ENEMY_BOREr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_CREATE_ENEMY_BORE\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_CREATE_ENEMY_BORE] [%d] [%s]onCache_CREATE_ENEMY_BORE r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

void onCache_BACK_OFF_CREATE_ENEMY_BORE(int i, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;

	if (type != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_BACK_OFF_CREATE_ENEMY_BORE\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, BACK_OFF_CREATE_ENEMY_BORE, pos);

	//k
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_BACK_OFF_CREATE_ENEMY_BORE\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_BACK_OFF_CREATE_ENEMY_BORE] [%d] [%s] onCache_BACK_OFF_CREATE_ENEMY_BOREr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_BACK_OFF_CREATE_ENEMY_BORE\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_BACK_OFF_CREATE_ENEMY_BORE] [%d] [%s]onCache_BACK_OFF_CREATE_ENEMY_BORE r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

void onCache_BORE_UPDATE_I(int i, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;

	if (type != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_BORE_UPDATE_I\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, BORE_UPDATE_I, pos);

	//k
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_BORE_UPDATE_I\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_BORE_UPDATE_I] [%d] [%s] onCache_BORE_UPDATE_Ir\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}


	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_BORE_UPDATE_I\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_BORE_UPDATE_I] [%d] [%s]onCache_BORE_UPDATE_I r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}

}
void onCache_CREATE_ENEMY(int row, int col, int type_, bool flash, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	WORD type = (WORD)row;
	WORD _j = (WORD)col;
	WORD _k = (WORD)type_;
	WORD _flash = (WORD)flash;

	if (type != row || _j != col || _k != type_) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_CREATE_ENEMY\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, CREATE_ENEMY, pos);

	//k
	PutWord(packet, type, pos);
	PutWord(packet, _j, pos);
	PutWord(packet, _k, pos);
	PutWord(packet, _flash, pos);
	PutSize(packet, pos);



	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_CREATE_ENEMY\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_CREATE_ENEMY] [%d] [%s] onCache_CREATE_ENEMYr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_CREATE_ENEMY\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_CREATE_ENEMY] [%d] [%s]onCache_CREATE_ENEMY r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}
void onCache_REMOVE_ENEMY_BORE_I(int i, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD type = (WORD)i;

	if (type != i) {
		log("[ERROR::DATA_TRANSFER]int into WORD in onCache_REMOVE_ENEMY_BORE_I\r\n\r\n\r\n");
	}

	//��װ����
	//Tag
	PutByte(packet, REMOVE_ENEMY_BORE_I, pos);

	//k
	PutWord(packet, type, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_REMOVE_ENEMY_BORE_I\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_REMOVE_ENEMY_BORE_I] [%d] [%s] onCache_REMOVE_ENEMY_BORE_Ir\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2

	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_REMOVE_ENEMY_BORE_I\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_REMOVE_ENEMY_BORE_I] [%d] [%s]onCache_REMOVE_ENEMY_BORE_I r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

void onCache_DEAL_WITH_PLAYER_BORE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;


	//��װ����
	//Tag
	PutByte(packet, DEAL_WITH_PLAYER_BORE, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_DEAL_WITH_PLAYER_BORE\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_DEAL_WITH_PLAYER_BORE] [%d] [%s] onCache_DEAL_WITH_PLAYER_BOREr\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_DEAL_WITH_PLAYER_BORE\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_DEAL_WITH_PLAYER_BORE] [%d] [%s]onCache_DEAL_WITH_PLAYER_BORE r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}



/*
OTHERS
friend: size:word->tag:byte
set player_type here
*/
void onCache_dPACKET_FRIEND(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//client
	int pos_1 = 2;
	int pos_2 = 2;

	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];

	//set player_type
	pClient1->player_type = 0;
	pClient2->player_type = 1;


	/*
	client1
	*/
	PutByte(content_to_1, dPACKET_FRIEND, pos_1);
	PutWord(content_to_1,(WORD)(pClient1->player_type),pos_1);
	PutSize(content_to_1, (WORD)pos_1);

	/*
	client2
	*/

	PutByte(content_to_2, dPACKET_FRIEND, pos_2);
        PutWord(content_to_2,(WORD)(pClient2->player_type),pos_2);
	PutSize(content_to_2, (WORD)pos_2);

	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_dPACKET_FRIEND\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
			pClient1->m_sendSize += pos_1;
			log("[CACHE::onCache_dPACKET_FRIEND] [%d] [%s]onCache_dPACKET_FRIEND r\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_dPACKET_FRIEND\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
			pClient2->m_sendSize += pos_2;
			log("[CACHE::onCache_dPACKET_FRIEND] [%d] [%s]onCache_dPACKET_FRIEND r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}


//Client
void onCache_FRAME_NO(WORD frame_num, sPCLIENT_DATA network) {
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	//��װ����
	//Tag
	PutByte(packet, FRAME_NO, pos);
	PutWord(packet, frame_num, pos);
	PutSize(packet, pos);

	if (network->m_sendSize + pos > dMAX_SOCK_BUFF) {
		log("[WARNING::OVERFLOW]client [%d] onCache_FRAME_NO\r\n", network->m_scok);
	}
	else {
		memcpy(&network->m_sendBuff[network->m_sendSize], packet, pos);
		network->m_sendSize += pos;
		log("[CACHE::onCache_FRAME_NO:%d] [%s] onCache_FRAME_NO\r\n\r\n", frame_num, network->m_IP);
	}
}


//SERVER
void onCache_FRAME_NO(WORD frame_num, sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	//��װ����
	//Tag
	PutByte(packet, FRAME_NO, pos);

	//k
	PutWord(packet, frame_num, pos);

	//size
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_FRAME_NO\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s] onCache_FRAME_NO\r\n\r\n", frame_num,pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_FRAME_NO\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s]onCache_FRAME_NO \r\n\r\n", frame_num,pClient2->m_IP);
		}
	}
}



/*
        for double

*/

void onCache_GAME_RUN_FRAME(Game& game,sPCLIENT_DATA pClient1,sPCLIENT_DATA pClient2,int frame_num){
	byte packet[dMAX_SOCK_BUFF];
       log("debug info ::start PutGameRunFrame\r\n") ;

//	int pos = 0;//wait to be deleted
	int pos = PutGameRunFrame(packet,game,frame_num);


       log("debug info ::end PutGameRunFrame\r\n") ;

	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_FRAME_NO\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s] onCache_FRAME_NO\r\n\r\n", frame_num,pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_FRAME_NO\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s]onCache_FRAME_NO \r\n\r\n", frame_num,pClient2->m_IP);
		}
	}
}

void onCache_GAME_WIN_FRAME(Game& game,sPCLIENT_DATA pClient1,sPCLIENT_DATA pClient2,int frame_num){


	byte packet[dMAX_SOCK_BUFF];
        int pos = PutGameWinFrame(packet,game,frame_num);


	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_FRAME_NO\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s] onCache_FRAME_NO\r\n\r\n", frame_num,pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_FRAME_NO\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s]onCache_FRAME_NO \r\n\r\n", frame_num,pClient2->m_IP);
		}
	}

}

void onCache_GAME_OVER_FRAME(Game& game,sPCLIENT_DATA pClient1,sPCLIENT_DATA pClient2,int frame_num){


	byte packet[dMAX_SOCK_BUFF];
        int pos = PutGameOverFrame(packet,game,frame_num);


	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCache_FRAME_NO\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s] onCache_FRAME_NO\r\n\r\n", frame_num,pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCache_FRAME_NO\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCache_FRAME_NO:%d] [%s]onCache_FRAME_NO \r\n\r\n", frame_num,pClient2->m_IP);
		}
	}

}








/*
onHandle
*/

/*
DATA_TRANSFER
*/
#ifdef _SERVER_
/*
����server���ԣ���������pClient���Ǹ����ݰ�����Դ
*/
void onHandle_PLAYER_MOVE(Game & game, sPCLIENT_DATA pClient) {
	game.player_move(pClient->player_type);
}


/*
size:word->tag:byte->dir:word
*/
void onHandle_SET_PLAYER_DIR(Game & game, sPCLIENT_DATA pClient) {
	//�õ�����
	int pos = 3;
	WORD dir = GetWord(pClient->m_recvBuff, pos);

	//�߼�����
	game.set_player_dir(pClient->player_type, (Direction)dir);



}

/*
size��word-��tag��byte
*/
void onHandle_CREATE_PLAYER_BULLET(Game & game, sPCLIENT_DATA pClient) {
	game.create_player_bullet(pClient->player_type);
}

#endif

#ifdef _CLIENT_

/*
�Կͻ��˶��ԣ�������Щ���ݰ��ķ�ʽ�������ñ�Client������Clientָ�����Ӧ����
*/
void onHandle_PLAYER_MOVE(Game & game, sPCLIENT_DATA pClient) {
	game.player_move(!(pClient->player_type));//˵���ǶԷ����ƶ�
}

void onHandle_SET_PLAYER_DIR(Game & game, sPCLIENT_DATA pClient) {
	//�õ�����
	int pos = 3;
	WORD dir = GetWord(pClient->m_recvBuff, pos);

	//�߼�����
	game.set_player_dir(!(pClient->player_type), (Direction)dir);

}

/*
size��word-��tag��byte;
client
*/
void onHandle_CREATE_PLAYER_BULLET(Game & game, sPCLIENT_DATA pClient) {
	game.create_player_bullet(!(pClient->player_type));
}
#endif // _CLIENT_






/*
OTHERS
*/
void onHandle_dPACKET_IS_READY(sPCLIENT_DATA pClient) {
	pClient->is_Ready = 1;
}


void onHandle_KILL_PLAYER_J(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;//������Ϊֱ�ӿ�ʼ��ȡ
	WORD j = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.create_big_explode(game.m_player[j].row(), game.m_player[j].col());
	game.remove_player(j);

}
void onHandle_CREATE_BONUS(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD type = GetWord(client->m_recvBuff, pos);
	WORD row = GetWord(client->m_recvBuff, pos);
	WORD col = GetWord(client->m_recvBuff, pos);


	//�����߼�
	game.m_bonus_living = true;
	game.m_bonus.create((BonusType)type, row, col);
}
void onHandle_KILL_ENEMY_K(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD k = GetWord(client->m_recvBuff, pos);


	//�����߼�
	game.m_count[i][(int)(game.m_enemy[k].type()) + 1] ++;
	game.create_small_explode(game.m_enemy[k].row(), game.m_enemy[k].col());
	game.remove_enemy(k);

}
void onHandle_REMOVE_PLAYER_BULLET_I_J(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD j = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.remove_player_bullet(i, j);

}

void onHandle_PLAYER_BULLET_HIT_PLAYER(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD j = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.m_player[!i].set_state(PlayerWink);
	game.remove_player_bullet(i, j);
}
void onHandle_PLAYER_BULLET_HIT_PLAYER_BULLET(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD j = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.create_small_explode(game.m_player_bullet[0][i].row(), game.m_player_bullet[0][i].col());
	game.remove_player_bullet(0, i);
	game.remove_player_bullet(1, j);


}
void onHandle_PLAYER_BULLET_HIT_ENEMY_BULLET(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD j = GetWord(client->m_recvBuff, pos);
	WORD k = GetWord(client->m_recvBuff, pos);


	//�����߼�
	game.remove_player_bullet(i, j);
	game.remove_enemy_bullet(k);

}
void onHandle_PLAYER_BULLET_HIT_FENCE(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD j = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.create_small_explode(game.m_player_bullet[i][j].row(), game.m_player_bullet[i][j].col());
	//				m_audio.play_sound(SoundBlockHitted);
	game.remove_player_bullet(i, j);
}
void onHandle_ALL_PLAYER_BULLET_MOVE(Game& game, sPCLIENT_DATA client) {
	game.player_bullet_update();
}

void onHandle_CREATE_ENEMY_BORE(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD cou = GetWord(client->m_recvBuff, pos);
	//�����߼�

	game.create_enemy_bore(cou);
}
void onHandle_BACK_OFF_CREATE_ENEMY_BORE(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD tmp = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.m_enemy_bore_living--;
	game.m_enemy_left++;
	game.m_last_enemy_bore_click = tmp;
}
void onHandle_BORE_UPDATE_I(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.m_enemy_bore[i].update();
}
void onHandle_CREATE_ENEMY(Game& game, sPCLIENT_DATA client) {
	//��ȡ����

	int pos = 3;

	WORD row = GetWord(client->m_recvBuff, pos);
	WORD col = GetWord(client->m_recvBuff, pos);
	WORD type = GetWord(client->m_recvBuff, pos);
	WORD flash = GetWord(client->m_recvBuff, pos);



	//�����߼�
	game.m_enemy[game.m_enemy_living].create((EnemyType)type, (bool)flash, row, col);
	game.m_enemy[game.m_enemy_living].add_level(game.m_enemy_level);
	game.m_enemy_living++;

}
void onHandle_REMOVE_ENEMY_BORE_I(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);

	//�߼�����
	game.remove_enemy_bore(i);
}
void onHandle_DEAL_WITH_PLAYER_BORE(Game& game, sPCLIENT_DATA client) {
	game.deal_with_player_bore();
}

void onHandle_REMOVE_ENEMY_BULLET(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);

	//�����߼�
	game.remove_enemy_bullet(i);
}

void onHandle_FRAME_NO(sPCLIENT_DATA fromClient) {
	//��ȡ����
	int pos = 3;
	WORD frame_num = GetWord(fromClient->m_recvBuff, pos);

	log("[INFO::FRAME_ARRIVE]from : %s frame_no : %d\r\n", fromClient->m_IP, frame_num);
}


void onHandle_dPACKET_FRIEND(Game& game) {
	//����Ϸ��got_friend״̬�ó�true
	game.got_friend = true;
}





bool onHandle_CONTROL_CENTER_FOR_IS_READY(sPCLIENT_DATA pClient){
	WORD msgSize;
	BYTE tag;
	

        while (pClient->m_recvSize > 0) {
                pClient->m_recvPos = 0;
                msgSize = GetWord(pClient->m_recvBuff, pClient->m_recvPos);
                tag = GetByte(pClient->m_recvBuff, pClient->m_recvPos);

                if (pClient->m_recvSize < msgSize) {
                        return false;
                }

                switch (tag)
                {
                case dPACKET_IS_READY:
                        onHandle_dPACKET_IS_READY(pClient);
                        break;
                default:
                	log("[WARNING::NO_MATCH_TYPE]in onHandle_CONTROL_CENTER_FOR_IS_READY\r\n\r\n");
		        break;
                }


                memmove(&pClient->m_recvBuff[0],
                        &pClient->m_recvBuff[msgSize],
                        pClient->m_recvSize - msgSize);
                pClient->m_recvSize -= msgSize;

        }
        return true;


}


bool onHandle_CONTROL_CENTER(Game& game, sPCLIENT_DATA pClient, sPCLIENT_DATA to_client) {
	WORD msgSize;
	BYTE tag;

	log("[READ::onHandle_CONTROL_CENTER]FROM:%s::%d \r\n\r\n",pClient->m_IP, pClient->m_scok);
	int num = 0;//һ�����ֻ�ܶ�һ��FRAME_NO���ݰ�

	while (pClient->m_recvSize > 0) {
		pClient->m_recvPos = 0;
		msgSize = GetWord(pClient->m_recvBuff, pClient->m_recvPos);
		tag = GetByte(pClient->m_recvBuff, pClient->m_recvPos);

		if (pClient->m_recvSize < msgSize) {
			return false;
		}

		/*
		���ݰ�����
		*/
		switch (tag)
		{
		case PLAYER_MOVE:
			onHandle_PLAYER_MOVE(game, pClient);
			//ת��һ��
//			if (to_client != NULL) {
	//			onCache_PLAYER_MOVE(to_client);
		//	}
			break;
		case SET_PLAYER_DIR:
			onHandle_SET_PLAYER_DIR(game, pClient);

		//	if (to_client != NULL) {
		//	//	int pos_ = 3;
			//	onCache_SET_PLAYER_DIR(to_client, (int)(GetWord(pClient->m_recvBuff, pos_)));
			//}
			break;
		case CREATE_PLAYER_BULLET:
			onHandle_CREATE_PLAYER_BULLET(game, pClient);
		//	if (to_client != NULL) {
		//		onCache_CREATE_PLAYER_BULLET(to_client);
		//	}
			break;
		case dPACKET_IS_READY:
			onHandle_dPACKET_IS_READY(pClient);
			break; 
		case FRAME_NO:
			onHandle_FRAME_NO(pClient);
			num += 1;
			if (num > 1) {
				//˵�����ǵڶ��ζ���FRAME_NO���ݰ���ֹͣ����������������ݡ�����Ϊһ��ֻ��һ֡�����ݰ�
				return true;
			}
			//onCache_FRAME_NO();
			//�����û�������������֡���ݰ���FRAME_NO���ݰ���������������ת��
		default:
			break;
		}


		memmove(&pClient->m_recvBuff[0],
			&pClient->m_recvBuff[msgSize],
			pClient->m_recvSize - msgSize);
		pClient->m_recvSize -= msgSize;

	}
	return true;
}


#ifdef _CLIENT_
bool onHandle_CONTROL_CENTER(Game& game, sPCLIENT_DATA pClient) {
	WORD msgSize;
	BYTE tag;
	int num = 0;//һ�����ֻ�ܶ�һ��FRAME_NO���ݰ�

	log("[READ::onHandle_CONTROL_CENTER]:%s \r\n\r\n", pClient->m_scok, pClient->m_IP);

	while (pClient->m_recvSize > 0) {
		pClient->m_recvPos = 0;
		msgSize = GetWord(pClient->m_recvBuff, pClient->m_recvPos);
		tag = GetByte(pClient->m_recvBuff, pClient->m_recvPos);

		if (pClient->m_recvSize < msgSize) {
			return false;
		}

		/*
		���ݰ�����
		*/
		switch (tag)
		{
		case KILL_PLAYER_J:
			onHandle_KILL_PLAYER_J(game, pClient);
			break;
		case CREATE_BONUS:
			onHandle_CREATE_BONUS(game, pClient);
			break;
		case KILL_ENEMY_K:
			onHandle_KILL_ENEMY_K(game, pClient);
			break;
		case REMOVE_PLAYER_BULLET_I_J:
			onHandle_REMOVE_PLAYER_BULLET_I_J(game, pClient);
			break;
		case PLAYER_BULLET_HIT_PLAYER:
			onHandle_PLAYER_BULLET_HIT_PLAYER(game, pClient);
			break;
		case PLAYER_BULLET_HIT_PLAYER_BULLET:
			onHandle_PLAYER_BULLET_HIT_PLAYER_BULLET(game, pClient);
			break;
		case PLAYER_BULLET_HIT_ENEMY_BULLET:
			onHandle_PLAYER_BULLET_HIT_ENEMY_BULLET(game, pClient);
			break;
		case PLAYER_BULLET_HIT_FENCE:
			onHandle_PLAYER_BULLET_HIT_FENCE(game, pClient);
			break;
		case ALL_PLAYER_BULLET_MOVE:
			onHandle_ALL_PLAYER_BULLET_MOVE(game, pClient);
			break;
		case CREATE_ENEMY_BORE:
			onHandle_CREATE_ENEMY_BORE(game, pClient);
			break;
		case BACK_OFF_CREATE_ENEMY_BORE:
			onHandle_BACK_OFF_CREATE_ENEMY_BORE(game, pClient);
			break;
		case BORE_UPDATE_I:
			onHandle_BORE_UPDATE_I(game, pClient);
			break;
		case CREATE_ENEMY:
			onHandle_CREATE_ENEMY(game, pClient);
			break;
		case REMOVE_ENEMY_BORE_I:
			onHandle_REMOVE_ENEMY_BORE_I(game, pClient);
			break;
		case DEAL_WITH_PLAYER_BORE:
			onHandle_DEAL_WITH_PLAYER_BORE(game, pClient);
			break;
		case REMOVE_ENEMY_BULLET:
			onHandle_REMOVE_ENEMY_BULLET(game, pClient);
			break;
		case dPACKET_FRIEND:
			onHandle_dPACKET_FRIEND(game);
			break;
		case PLAYER_MOVE:
			onHandle_PLAYER_MOVE(game, pClient);
			break;
		case SET_PLAYER_DIR:
			onHandle_SET_PLAYER_DIR(game, pClient);
			break;
		case CREATE_PLAYER_BULLET:
			onHandle_CREATE_PLAYER_BULLET(game, pClient);
			break;
		case dPACKET_IS_READY:
			onHandle_dPACKET_IS_READY(pClient);
		case FRAME_NO:
			onHandle_FRAME_NO(pClient);
			num += 1;
			if (num > 1) {
				//˵�����ǵڶ��ζ���FRAME_NO���ݰ���ֹͣ����������������ݡ�����Ϊһ��ֻ��һ֡�����ݰ�
				return true;
			}
		case CALL_ENEMY_BULLET_HIT_TILE:
			onHandle_CALL_ENEMY_BULLET_HIT_TILE(game);
			break;
		case CALL_ENEMY_BULLET_HIT_FENCE:
			onHandle_CALL_ENEMY_BULLET_HIT_FENCE(game);
			break;
		case CALL_ENEMY_BULLET_UPDATE:
			onHandle_CALL_ENEMY_BULLET_UPDATE(game);
			break;
		case CALL_PLAYER_BULLET_HIT_TILE:
			onHandle_CALL_PLAYER_BULLET_HIT_TILE(game);
			break;
		case CALL_PLAYER_BULLET_UPDATE:
			onHandle_CALL_PLAYER_BULLET_UPDATE(game);
			break;
		case CALL_DEAL_WITH_PLAYER:
			onHandle_CALL_DEAL_WITH_PLAYER(game);
			break;
		case CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK:
			onHandle_CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK(game);
			break;
		case CALL_DEAL_WITH_ENEMY_LEFT:
			onHandle_CALL_DEAL_WITH_ENEMY_LEFT(game, pClient);
			break;
		case CALL_DEAL_WITH_ENEMY_RIGHT:
			onHandle_CALL_DEAL_WITH_ENEMY_RIGHT(game, pClient);
			break;
		case CALL_DEAL_WITH_ENEMY_UP:
			onHandle_CALL_DEAL_WITH_ENEMY_UP(game, pClient);
			break;
		case CALL_DEAL_WITH_ENEMY_DOWN:
			onHandle_CALL_DEAL_WITH_ENEMY_DOWN(game, pClient);
			break;
		case CALL_DEAL_WITH_ENEMY_SET_DIR:
			onHandle_CALL_DEAL_WITH_ENEMY_SET_DIR(game, pClient);
			break;
		case CALL_DEAL_WITH_ENEMY_UPDATE_ENEMY_FREEZING:
			onHandle_CALL_DEAL_WITH_ENEMY_UPDATE_ENEMY_FREEZING(game);
			break;
		case CALL_DEAL_WITH_ENEMY_BORE_UPDATE:
			onHandle_CALL_DEAL_WITH_ENEMY_BORE_UPDATE(game);
			break;
		case CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY:
			onHandle_CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY(game);
			break;
		case CALL_ENEMY_SET_DIR:
			onHandle_CALL_ENEMY_SET_DIR(game, pClient);
			break;
		case CALL_ENEMY_UPDATE_ATTACK:
			onHandle_CALL_ENEMY_UPDATE_ATTACK(game, pClient);
			break;
		default:
			log("[WARNING::HANDLE]sth not be recongnized \r\n\r\n\r\n");
			break;
		}


		/*
		��һ�����ݰ�����һ��д������
		*/
		memmove(&pClient->m_recvBuff[0],
			&pClient->m_recvBuff[msgSize],
			pClient->m_recvSize - msgSize);
		pClient->m_recvSize -= msgSize;

	}
	return true;
}


#endif

/*
new added here
*/
void onCache_CALL_CENTER(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, char type) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;

	//��װ����
	//Tag
	PutByte(packet, type, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCacheCall\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s] onCacheCall_%c\r\n\r\n", pClient1->m_scok, pClient1->m_IP,type);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCacheCall\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s]onCacheCall_%c \r\n\r\n", pClient2->m_scok, pClient2->m_IP,type);
		}
	}

}
/*
new added here
*/
void onCache_CALL_CENTER_I(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, char type, int i) {
	//���͸������û�
	char packet[dMAX_SOCK_BUFF];
	int pos = 2;
	WORD i_ = (WORD)i;

	//��װ����
	//Tag
	PutByte(packet, type, pos);
	PutWord(packet, i_, pos);
	PutSize(packet, pos);


	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCacheCall\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s] onCacheCall_%c\r\n\r\n", pClient1->m_scok, pClient1->m_IP,type);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCacheCall\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s]onCacheCall_%c \r\n\r\n", pClient2->m_scok, pClient2->m_IP,type);
		}
	}

}

void onCache_CALL_ENEMY_BULLET_HIT_TILE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) {
	onCache_CALL_CENTER(pClient1, pClient2, CALL_ENEMY_BULLET_HIT_TILE);
}



void onCache_CALL_ENEMY_BULLET_HIT_FENCE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_ENEMY_BULLET_HIT_FENCE);}
void onCache_CALL_ENEMY_BULLET_UPDATE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_ENEMY_BULLET_UPDATE); }
void onCache_CALL_PLAYER_BULLET_HIT_TILE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_PLAYER_BULLET_HIT_TILE); }
void onCache_CALL_PLAYER_BULLET_UPDATE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_PLAYER_BULLET_UPDATE); }
void onCache_CALL_DEAL_WITH_PLAYER(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_DEAL_WITH_PLAYER); }
void onCache_CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK); }
void onCache_CALL_DEAL_WITH_ENEMY_LEFT(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i) { onCache_CALL_CENTER_I(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_LEFT, i); }
void onCache_CALL_DEAL_WITH_ENEMY_RIGHT(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i) { onCache_CALL_CENTER_I(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_RIGHT, i); }
void onCache_CALL_DEAL_WITH_ENEMY_UP(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i) { onCache_CALL_CENTER_I(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_UP, i); }
void onCache_CALL_DEAL_WITH_ENEMY_DOWN(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i) { onCache_CALL_CENTER_I(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_DOWN, i); }
void onCache_CALL_DEAL_WITH_ENEMY_UPDATE_ENEMY_FREEZING(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_UPDATE_ENEMY_FREEZING); }
void onCache_CALL_DEAL_WITH_ENEMY_BORE_UPDATE(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_DEAL_WITH_ENEMY_BORE_UPDATE); }
void onCache_CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2) { onCache_CALL_CENTER(pClient1, pClient2, CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY); }
//
//void onCache_CALL_DEAL_WITH_ENEMY_SET_DIR(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i, int dir) {
//	int pos = 2;
//	WORD i_ = (WORD)i;
//	WORD dir_ = (WORD)dir;
//	char packet[dMAX_SOCK_BUFF];
//
//
//	PutByte(packet, CALL_DEAL_WITH_ENEMY_SET_DIR, pos);
//	PutWord(packet, i_, pos);
//	PutWord(packet, dir_, pos);
//	PutSize(packet, pos);
//	//���浽�����û��Ļ�����
//	//client1
//	if (pClient1 != NULL) {
//		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
//			log("[WARNING::OVERFLOW]client [%d] onCacheCall\r\n", pClient1->m_scok);
//		}
//		else {
//			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
//			pClient1->m_sendSize += pos;
//			log("[CACHE::onCacheCall] [%d] [%s] onCacheCallr\n\r\n", pClient1->m_scok, pClient1->m_IP);
//		}
//	}
//
//	//client2
//	if (pClient2 != NULL) {
//		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
//			log("[WARNING::OVERFLOW]client [%d]onCacheCall\r\n", pClient2->m_scok);
//		}
//		else
//		{
//			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
//			pClient2->m_sendSize += pos;
//			log("[CACHE::onCacheCall] [%d] [%s]onCacheCall r\n\r\n", pClient2->m_scok, pClient2->m_IP);
//		}
//	}
//
//
//}

void onCache_CALL_ENEMY_SET_DIR(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i, int dir) {
	int pos = 2;
	WORD i_ = (WORD)i;
	WORD dir_ = (WORD)dir;
	char packet[dMAX_SOCK_BUFF];


	PutByte(packet, CALL_ENEMY_SET_DIR, pos);
	PutWord(packet, i_, pos);
	PutWord(packet, dir_, pos);
	PutSize(packet, pos);
	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCacheCall\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s] onCacheCall_ENEMY_SET_dIR\r\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCacheCall\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s]onCacheCall_ENEMY_SET_DIR\r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

void onCache_CALL_ENEMY_UPDATE_ATTACK(sPCLIENT_DATA pClient1, sPCLIENT_DATA pClient2, int i) {
	int pos = 2;
	WORD i_ = (WORD)i;
	char packet[dMAX_SOCK_BUFF];


	PutByte(packet, CALL_ENEMY_UPDATE_ATTACK, pos);
	PutWord(packet, i_, pos);
	PutSize(packet, pos);
	//���浽�����û��Ļ�����
	//client1
	if (pClient1 != NULL) {
		if (pClient1->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d] onCacheCall\r\n", pClient1->m_scok);
		}
		else {
			memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], packet, pos);
			pClient1->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s] onCacheCall_ENEMY_UPDATE_ATTACK\r\n\r\n", pClient1->m_scok, pClient1->m_IP);
		}
	}

	//client2
	if (pClient2 != NULL) {
		if (pClient2->m_sendSize + pos > dMAX_SOCK_BUFF) {
			log("[WARNING::OVERFLOW]client [%d]onCacheCall\r\n", pClient2->m_scok);
		}
		else
		{
			memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], packet, pos);
			pClient2->m_sendSize += pos;
			log("[CACHE::onCacheCall] [%d] [%s]onCacheCall_ENEMY_UPDATE_ATTACK \r\n\r\n", pClient2->m_scok, pClient2->m_IP);
		}
	}
}

#ifdef _CLIENT_
void onHandle_CALL_ENEMY_BULLET_HIT_TILE(Game& game) { game.enemy_bullet_hit_tile(); }
void onHandle_CALL_ENEMY_BULLET_HIT_FENCE(Game& game) { game.enemy_bullet_hit_fence(); }
void onHandle_CALL_ENEMY_BULLET_UPDATE(Game& game) { game.enemy_bullet_update(); }
void onHandle_CALL_PLAYER_BULLET_HIT_TILE(Game& game) { game.player_bullet_hit_tile(); }
void onHandle_CALL_PLAYER_BULLET_UPDATE(Game& game) { game.player_bullet_update(); }
void onHandle_CALL_DEAL_WITH_PLAYER(Game& game) { game.deal_with_player(); }
void onHandle_CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK(Game& game) { game.deal_with_enemy_update_win_delay_click(); }
void onHandle_CALL_DEAL_WITH_ENEMY_UPDATE_ENEMY_FREEZING(Game& game) { game.deal_with_enemy_update_enemy_freezing(); }
void onHandle_CALL_DEAL_WITH_ENEMY_BORE_UPDATE(Game& game) { game.deal_with_enemy_bore_update(); }
void onHandle_CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY(Game& game) { game.deal_with_bonus_explode_message_delay(); }

void onHandle_CALL_DEAL_WITH_ENEMY_LEFT(Game& game, sPCLIENT_DATA client) {
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	game.deal_with_enemy_left((int)i);
}
void onHandle_CALL_DEAL_WITH_ENEMY_RIGHT(Game& game, sPCLIENT_DATA client) {
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	game.deal_with_enemy_right((int)i);
}
void onHandle_CALL_DEAL_WITH_ENEMY_UP(Game& game, sPCLIENT_DATA client) {
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	game.deal_with_enemy_up((int)i);
}

void onHandle_CALL_DEAL_WITH_ENEMY_DOWN(Game& game, sPCLIENT_DATA client) {
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	game.deal_with_enemy_down((int)i);
}

void onHandle_CALL_DEAL_WITH_ENEMY_SET_DIR(Game& game, sPCLIENT_DATA client) {
	int pos = 3;
	WORD index = GetWord(client->m_recvBuff, pos);
	WORD dir = GetWord(client->m_recvBuff, pos);

	game.deal_with_enemy_set_dir((int)index, (int)dir);

}

void onHandle_CALL_ENEMY_SET_DIR(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);
	WORD dir = GetWord(client->m_recvBuff, pos);

	//�߼�����
	game.m_enemy[i].set_dir((Direction)dir);//�������������򣬵����ܻ���ԭ������
}

void onHandle_CALL_ENEMY_UPDATE_ATTACK(Game& game, sPCLIENT_DATA client) {
	//��ȡ����
	int pos = 3;
	WORD i = GetWord(client->m_recvBuff, pos);

	//�߼�����
	game.m_enemy[i].update();
	game.enemy_try_attack(i);
}
#endif // CLIENT
