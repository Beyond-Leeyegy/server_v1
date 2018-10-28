/*
	FILE:meta_data.cpp

*/


#include "def.h"
#include "meta_data.h"
#include "read_write_on_bit.h"
#include <cstring>
#include "utils.h"

void PutByte(char *pPacket, BYTE value, int & nPos) {

	*(BYTE *)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

BYTE GetByte(char* pPacket, int& nPos) {
	BYTE value = *(BYTE *)(pPacket + nPos);
	nPos += sizeof(BYTE);
	return value;
}


void PutWord(char *pPacket, WORD value, int & nPos) {
	*(WORD *)(pPacket + nPos) = value;
	nPos += sizeof(WORD);
}

WORD GetWord(char* pPacket, int& nPos) {
	WORD value = *(WORD *)(pPacket + nPos);
	nPos += sizeof(WORD);
	return value;
}

void PutDword(char *pPacket, DWORD value, int & nPos) {
	*(DWORD *)(pPacket + nPos) = value;
	nPos += sizeof(DWORD);
}



DWORD GetDword(char* pPacket, int &nPos) {
	DWORD value = *(DWORD *)(pPacket + nPos);
	nPos += sizeof(DWORD);
	return value;
}


void static PutInteger(char *pPacket, int value, int & nPos) {
	*(int *)(pPacket + nPos) = value;
	nPos += sizeof(int);
}



int  static GetInteger(char* pPacket, int &nPos) {
	int value = *(int*)(pPacket + nPos);
	nPos += sizeof(int);
	return value;
}

void PutShort(char *pPacket, short value, int & nPos) {
	*(short *)(pPacket + nPos) = value;
	nPos += sizeof(short);
}

short GetShort(char* pPacket, int& nPos) {
	short value = *(short*)(pPacket + nPos);
	nPos += sizeof(short);
	return value;
}

void PutString(char *pPacket, char *str, int & nPos) {
	*(WORD*)(pPacket + nPos) = strlen(str);
	nPos += sizeof(WORD);
	memcpy(pPacket + nPos, str, strlen(str));
	nPos += strlen(str);
}

void GetString(char* pPacket, char *pBuffer, int &nPos) {
	WORD length = *(WORD*)(pPacket + nPos);
	nPos += sizeof(WORD);
	memcpy(pBuffer, pPacket + nPos, length);
	*(pBuffer + length) = '\0';
	nPos += length;
}

void PutSize(char *pPacket, WORD nPos) {
	*(WORD *)pPacket = nPos;
}



/*
	write on the end pos
	and after done writing
	return the now end pos
*/
void PutBore(byte* packet,Bore& bore,int& end){
	WriteDataToBuffer(packet, bore.m_width, end , 6, end);//width
	WriteDataToBuffer(packet, bore.m_height, end, 6, end);//width
	WriteDataToBuffer(packet, bore.m_hot_row, end, 8, end);//width
	WriteDataToBuffer(packet, bore.m_hot_col, end, 8, end);//width
	WriteDataToBuffer(packet, bore.m_row, end, 10, end);
	WriteDataToBuffer(packet, bore.m_col, end, 10, end);

	log("----------Bore-------\r\n");
	log("wid:%d\r\n",bore.m_width);
	log("hei:%d\r\n",bore.m_height);
	log("hor_row:%d\r\n",bore.m_hot_row);
	log("hot_col:%d\r\n",bore.m_hot_col);
	log("row:%d\r\n",bore.m_row);
	log("col:%d\r\n",bore.m_col);
	log("-------end Bore------\r\n\r\n");
}

void PutBonus(byte* packet ,Bonus& bonus,int & end){
        WriteDataToBuffer(packet, bonus.m_width, end , 6, end);//width
        WriteDataToBuffer(packet, bonus.m_height, end, 6, end);//width
        WriteDataToBuffer(packet, bonus.m_hot_row, end, 8, end);//width
        WriteDataToBuffer(packet, bonus.m_hot_col, end, 8, end);//width
        WriteDataToBuffer(packet, bonus.m_row, end, 10, end);
        WriteDataToBuffer(packet, bonus.m_col, end, 10, end);
	WriteDataToBuffer(packet, (int)bonus.m_type, end , 3, end);//width


        log("----------Bonus-------\r\n");
        log("wid:%d\r\n",bonus.m_width);
        log("hei:%d\r\n",bonus.m_height);
        log("hor_row:%d\r\n",bonus.m_hot_row);
        log("hot_col:%d\r\n",bonus.m_hot_col);
        log("row:%d\r\n",bonus.m_row);
        log("col:%d\r\n",bonus.m_col);
	log("type:%d\r\n",bonus.m_type);
        log("-------end Bonus------\r\n\r\n");

}


void PutBullet(byte* packet,Bullet& bullet,int& end){
        WriteDataToBuffer(packet, bullet.m_width, end , 6, end);//width
        WriteDataToBuffer(packet, bullet.m_height, end, 6, end);//width
        WriteDataToBuffer(packet, bullet.m_hot_row, end, 8, end);//width
        WriteDataToBuffer(packet, bullet.m_hot_col, end, 8, end);//width
        WriteDataToBuffer(packet, bullet.m_row, end, 10, end);
        WriteDataToBuffer(packet, bullet.m_col, end, 10, end);
        WriteDataToBuffer(packet, (int)bullet.m_dir, end , 2, end);//width
        WriteDataToBuffer(packet, (int)bullet.m_level, end, 2, end);//width



        log("----------Bullet-------\r\n");
        log("wid:%d\r\n",bullet.m_width);
        log("hei:%d\r\n",bullet.m_height);
        log("hor_row:%d\r\n",bullet.m_hot_row);
        log("hot_col:%d\r\n",bullet.m_hot_col);
        log("row:%d\r\n",bullet.m_row);
        log("col:%d\r\n",bullet.m_col);
	log("dir:%d\r\n",bullet.m_dir);
	log("level:%d\r\n",bullet.m_level);
        log("-------end Bullet------\r\n\r\n");


}

void PutTile(byte* packet,Tile& tile,int& end){
        WriteDataToBuffer(packet, (int)tile.m_data[0], end , 4, end);//width
        WriteDataToBuffer(packet, (int)tile.m_data[1], end , 4, end);
        WriteDataToBuffer(packet, (int)tile.m_data[2], end , 4, end);
        WriteDataToBuffer(packet, (int)tile.m_data[3], end , 4, end);
        WriteDataToBuffer(packet, (int)tile.m_data[4], end , 4, end);



        log("----------Tile-------\r\n");
        log("da1:%d\r\n",(int)tile.m_data[0]);
        log("da2:%d\r\n",(int)tile.m_data[1]);
        log("da3:%d\r\n",(int)tile.m_data[2]);
        log("da4:%d\r\n",(int)tile.m_data[3]);
        log("da5:%d\r\n",(int)tile.m_data[4]);
        log("-------end Tile------\r\n\r\n");

}


void PutFrame_No(byte* packet ,int & frame_size,int frame_num,char type){
	//deal with lastly
	//end : the total bits we used in this frame including the FRAME_NO
	int end = 0;
        WriteDataToBuffer(packet,(int)((frame_size/8)+1), 0, 10, end);
        WriteDataToBuffer(packet,type, end, 8, end);
        WriteDataToBuffer(packet, frame_num, end, 16, end);


        log("----------FrameNo-------\r\n");
        log("size-bytes:%d\r\n",(int)(frame_size/8+1));
        log("type:%c\r\n",type);
        log("frame_num:%d\r\n",frame_num);
        log("-------end FrameNo------\r\n\r\n");

}

void PutPlayer(byte* packet,Player& player,int& end){
        WriteDataToBuffer(packet, player.m_width, end , 6, end);//width
        WriteDataToBuffer(packet, player.m_height, end, 6, end);//width
        WriteDataToBuffer(packet, player.m_hot_row, end, 8, end);//width
        WriteDataToBuffer(packet, player.m_hot_col, end, 8, end);//width
        WriteDataToBuffer(packet, (int)player.m_level, end, 2, end);
        WriteDataToBuffer(packet, player.m_life_left, end, 8, end);
        WriteDataToBuffer(packet, player.m_row, end, 10, end);
        WriteDataToBuffer(packet, player.m_col, end, 10, end);
        WriteDataToBuffer(packet, (int)player.m_dir, end , 2, end);//width
        WriteDataToBuffer(packet, (int)player.m_shield, end, 1, end);//width



        log("----------Player-------\r\n");
        log("wid:%d\r\n",player.m_width);
        log("hei:%d\r\n",player.m_height);
        log("hor_row:%d\r\n",player.m_hot_row);
        log("hot_col:%d\r\n",player.m_hot_col);
        log("row:%d\r\n",player.m_row);
        log("col:%d\r\n",player.m_col);
	log("level:%d\r\n",(int)player.m_level);
	log("life:%d\r\n",player.m_life_left);
	log("dir:%d\r\n",(int)player.m_dir);
	log("shield:%d\r\n",(int)player.m_shield);
        log("-------end Player------\r\n\r\n");

}


void PutEnemy(byte* packet,Enemy& enemy,int& end){
        WriteDataToBuffer(packet, enemy.m_width, end , 6, end);//width
        WriteDataToBuffer(packet, enemy.m_height, end, 6, end);//width
        WriteDataToBuffer(packet, enemy.m_hot_row, end, 8, end);//width
        WriteDataToBuffer(packet, enemy.m_hot_col, end, 8, end);//width
        WriteDataToBuffer(packet, (int)enemy.m_level, end, 2, end);
        WriteDataToBuffer(packet, enemy.m_life_left, end, 8, end);
        WriteDataToBuffer(packet, enemy.m_row, end, 10, end);
        WriteDataToBuffer(packet, enemy.m_col, end, 10, end);
        WriteDataToBuffer(packet, (int)enemy.m_dir, end , 2, end);//width
        WriteDataToBuffer(packet, (int)enemy.m_shield, end, 1, end);//width
        WriteDataToBuffer(packet, (int)enemy.m_type, end , 2, end);//width
        WriteDataToBuffer(packet, (int)enemy.m_flash, end, 1, end);//width


        log("----------Enemy-------\r\n");
        log("wid:%d\r\n",enemy.m_width);
        log("hei:%d\r\n",enemy.m_height);
        log("hor_row:%d\r\n",enemy.m_hot_row);
        log("hot_col:%d\r\n",enemy.m_hot_col);
        log("row:%d\r\n",enemy.m_row);
        log("col:%d\r\n",enemy.m_col);
        log("level:%d\r\n",(int)enemy.m_level);
        log("life:%d\r\n",enemy.m_life_left);
        log("dir:%d\r\n",(int)enemy.m_dir);
        log("shield:%d\r\n",(int)enemy.m_shield);
	log("type:%d\r\n",(int)enemy.m_type);
	log("flash:%d\r\n",(int)enemy.m_flash);
        log("-------end Enemy------\r\n\r\n");


}

void PutExplode(byte* packet,Explode& explode,int& end){
        WriteDataToBuffer(packet, explode.m_width, end , 6, end);//width
        WriteDataToBuffer(packet, explode.m_height, end, 6, end);//width
        WriteDataToBuffer(packet, explode.m_hot_row, end, 8, end);//width
        WriteDataToBuffer(packet, explode.m_hot_col, end, 8, end);//width
        WriteDataToBuffer(packet, explode.m_row, end, 10, end);
        WriteDataToBuffer(packet, explode.m_col, end, 10, end);
        WriteDataToBuffer(packet, (int)explode.m_type,end,2,end);

        log("----------Explode-------\r\n");
        log("wid:%d\r\n",explode.m_width);
        log("hei:%d\r\n",explode.m_height);
        log("hor_row:%d\r\n",explode.m_hot_row);
        log("hot_col:%d\r\n",explode.m_hot_col);
        log("row:%d\r\n",explode.m_row);
        log("col:%d\r\n",explode.m_col);
        
	log("type:%d\r\n",(int)explode.m_type);
	log("-------end Explode------\r\n\r\n");



}

void PutGame(byte* packet,Game& game ,int & end){
     
	
       log("debug info ::start writeSetting with end pos:%d\r\n",end) ;
	//setting
        WriteDataToBuffer(packet,(int)game.m_game_state, end , 3, end);//width
        WriteDataToBuffer(packet, (int)game.m_level, end, 4, end);//width
        WriteDataToBuffer(packet,(int) game.m_enemy_level, end, 4, end);//width
        WriteDataToBuffer(packet,(int) game.m_player_level[0], end, 4, end);//width
        WriteDataToBuffer(packet, (int)game.m_player_level[1], end, 4, end);


        log("----------Game::setting-------\r\n");
        log("state:%d\r\n",game.m_game_state);
        log("enemy_level:%d\r\n",game.m_enemy_level);
        log("player1 level:%d\r\n",game.m_player_level[0]);
        log("player2 level:%d\r\n",game.m_player_level[1]);
        log("level:%d\r\n",(int)game.m_level);

        for(int i = 0;i<2;i++){
                for(int j = 0;j<4;j++){
                   WriteDataToBuffer(packet, game.m_count[i][j], end, 5, end);
                  log("count:%d\r\n",game.m_count[i][j]);
		}
        }

	log("------end Game::setting-\r\n\r\n");

       log("debug info ::end writeSetting;start writePlayer with end pos:%d\r\n",end) ;
        //player
        WriteDataToBuffer(packet, (int)game.m_player_freezing, end, 1, end);
        
	log("player freezing:%d\r\n",(int)game.m_player_freezing);
	
	PutPlayer(packet,game.m_player[0],end);
        PutPlayer(packet,game.m_player[1],end);
        WriteDataToBuffer(packet, (int)game.m_player_living[0], end, 1, end);
        WriteDataToBuffer(packet, (int)game.m_player_living[1], end, 1, end);
        WriteDataToBuffer(packet, game.m_player_life_left[0], end, 8, end);
        WriteDataToBuffer(packet, game.m_player_life_left[1], end, 8, end);
	
	log("----------Player-related-------\r\n");
        log("liv1:%d\r\n",game.m_player_living[0]);
        log("liv2:%d\r\n",game.m_player_living[1]);
        log("left1:%d\r\n",game.m_player_life_left[0]);
        log("left2:%d\r\n",game.m_player_life_left[1]);
        log("-------end Player-related------\r\n\r\n");

	


       log("debug info ::end writePlayer;start writeEnemy with end pos:%d\r\n",end) ;
        //enemy
        WriteDataToBuffer(packet, (int)game.m_enemy_freezing, end, 1, end);
        WriteDataToBuffer(packet, game.m_enemy_living, end, 5, end);
        for(int i = 0 ;i<game.m_enemy_living;i++){
                PutEnemy(packet,game.m_enemy[i],end);
        }
        WriteDataToBuffer(packet, game.m_enemy_left, end, 5, end);

       log("debug info ::end writeEnemy;start writePlayerBullet with end pos:%d\r\n",end) ;
        //Player Bullet
        WriteDataToBuffer(packet, (int)game.m_player_bullet_living[0], end, 3, end);
        for(int i = 0 ;i<game.m_player_bullet_living[0];i++){
                PutBullet(packet,game.m_player_bullet[0][i],end);
        }

        WriteDataToBuffer(packet, (int)game.m_player_bullet_living[1], end, 3, end);
        for(int i = 0 ;i<game.m_player_bullet_living[1];i++){
                PutBullet(packet,game.m_player_bullet[1][i],end);
        }

       log("debug info ::end writePlayerBullet;start writeEnemyBullet with end pos:%d\r\n",end) ;
        //Enemy Bullet
        WriteDataToBuffer(packet, (int)game.m_enemy_bullet_living, end, 6, end);
        for(int i = 0 ;i<game.m_enemy_bullet_living;i++){
                PutBullet(packet,game.m_enemy_bullet[i],end);
        }

       log("debug info ::end writeEnemyBullet;start write PlayerBore with end pos:%d\r\n",end) ;
        //Player Bore
        WriteDataToBuffer(packet, (int)game.m_player_bore_living[0], end, 1, end);
        WriteDataToBuffer(packet, (int)game.m_player_bore_living[1], end, 1, end);
        PutBore(packet,game.m_player_bore[0],end);
        PutBore(packet,game.m_player_bore[1],end);

       log("debug info ::end writePlayerBore;start writeEnemyBore with end pos:%d\r\n",end) ;
        //Enemy Bore
        WriteDataToBuffer(packet, (int)game.m_enemy_bore_living, end, 2, end);
        for(int i = 0;i<game.m_enemy_bore_living;i++){
                PutBore(packet,game.m_enemy_bore[i],end);
        }

       log("debug info ::end writeENbore;start writeBigEXPODLE with end pos:%d\r\n",end) ;
        //big explode
        WriteDataToBuffer(packet, (int)game.m_big_explode_living, end, 5, end);
        for(int i = 0;i<game.m_big_explode_living;i++){
               PutExplode(packet,game.m_big_explode[i],end);
        }

       log("debug info ::end writeBigeX;start writeSmallEx with end pos:%d\r\n",end) ;
        //small explode
        WriteDataToBuffer(packet, (int)game.m_small_explode_living, end,5, end);       for(int i = 0;i<game.m_small_explode_living;i++){
                PutExplode(packet,game.m_small_explode[i],end);
        }
     
 log("debug info ::end writeSmallEx;start writeHawkExp with end pos:%d\r\n",end) ;
	
        //hawk explode
        WriteDataToBuffer(packet, (int)game.m_hawk_explode_living, end, 1, end);
        PutExplode(packet,game.m_hawk_explode,end);


 log("debug info ::end writehawkEx;start writeBonus with end pos:%d\r\n",end) ;

         //bonus
        WriteDataToBuffer(packet, (int)game.m_bonus_living, end, 1, end);
        PutBonus(packet,game.m_bonus,end);


 log("debug info ::end writeBonus ;return\r\n") ;


}


void PutMap(byte* packet,Game& game,int & end){
        WriteDataToBuffer(packet, (int)game.m_map.m_protected, end, 1, end);
        WriteDataToBuffer(packet, (int)game.m_map.m_bare, end, 1, end);

        //Tile[13*13]

        for(int i =  0;i<13*13;i++){
              PutTile(packet,game.m_map.m_tile_array[i],end);
        }
}

////return the bytes this frame use
//int PutGameRunFrame(byte* packet,Game game,int frame_num){
//        int frame_size;
//        int end = 34;
//
//	log("debug info::start PutMap end pos:%d\r\n",end);
//	//map
//	PutMap(packet,game,end);
//
//	log("debug info::end putmap;start putgame with end pos:%d\r\n",end);
//        //game  
//        PutGame(packet,game ,end);
//
//        frame_size =(int)((end/8)+1);
//        //FRAME_NO
//        PutFrame_No(packet,end,frame_num,GAME_RUN_FRAME);
//
//        return frame_size;
//
// }
//     
	


//void PutMap(byte* packet,Game game,int & end){
//        WriteDataToBuffer(packet, (int)game.m_map.m_protected, end, 1, end);
//        WriteDataToBuffer(packet, (int)game.m_map.m_bare, end, 1, end);
//
//        //Tile[13*13]
//
//        for(int i =  0;i<13*13;i++){
//              PutTile(packet,game.m_map.m_tile_array[i],end);
//        }
//}
//
////return the bytes this frame use
//int PutGameRunFrame(byte* packet,Game game,int frame_num){
//        int frame_size;
//        int end = 34;
//        
//
//
//	log("debug info::start PutMap end pos:%d\r\n",end);
//	//map
//        
//
//	PutMap(packet,game,end);
//
//	log("debug info::end putmap;start putgame with end pos:%d\r\n",end);
//        //game 
//        
//        //hawk explode
//        WriteDataToBuffer(packet, (int)game.m_hawk_explode_living, end, 1, end);
//        PutExplode(packet,game.m_hawk_explode,end);
//
//         //bonus
//        WriteDataToBuffer(packet, (int)game.m_bonus_living, end, 1, end);
//        PutBonus(packet,game.m_bonus,end);
//}


//void PutMap(byte* packet,Game game,int & end){
//        WriteDataToBuffer(packet, (int)game.m_map.m_protected, end, 1, end);
//        WriteDataToBuffer(packet, (int)game.m_map.m_bare, end, 1, end);
//
//        //Tile[13*13]
//
//        for(int i =  0;i<13*13;i++){
//              PutTile(packet,game.m_map.m_tile_array[i],end);
//        }
//}

//return the bytes this frame use
int PutGameRunFrame(byte* packet,Game& game , int frame_num){
        int frame_size;
        int end = 34;

	log("debug info::start PutMap end pos:%d\r\n",end);
	//map
	PutMap(packet,game,end);

	log("debug info::end putmap;start putgame with end pos:%d\r\n",end);
        //game 
        PutGame(packet,game ,end);
	log("debug info::end putgame;start putframe_no with end pos:%d\r\n",end);
        frame_size =(int)((end/8)+1);
        //FRAME_NO
        PutFrame_No(packet,end,frame_num,GAME_RUN_FRAME);

	log("debug info::end put_frame_no;return with end pos:%d\r\n",end);
        return frame_size;

}

int PutGameWinFrame(byte * packet,Game& game,int frame_num){
        int frame_size;
        int end = 34;

        WriteDataToBuffer(packet, (int)game.m_game_state, end, 3, end);
        WriteDataToBuffer(packet, (int)game.m_win_click, end, 9, end);

        frame_size = (int)((end/8)+1);

        //FRAME_NO
        PutFrame_No(packet,end,frame_num,GAME_WIN_FRAME);

        return frame_size;

}

int PutGameOverFrame(byte * packet,Game& game,int frame_num){
        int frame_size;
        int end = 34;

        WriteDataToBuffer(packet, (int)game.m_game_state, end, 3, end);

        frame_size = (int)((end/8)+1);

        //FRAME_NO
        PutFrame_No(packet,end,frame_num,GAME_OVER_FRAME);

        return frame_size;

}



