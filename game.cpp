#include <iostream>
#include "game.hpp"
#include "data_protocal.h"
#include "network.h"
#include "utils.h"
#define row_off 10
#define col_off 8 



//#define debug(info) printf(info)
//#define debug1(info,arg1) printf(info,arg1)
//#define debug2(info,arg1,arg2) printf(info,arg1,arg2)
//#define debug3(info,arg1,arg2,arg3) printf(info,arg1,arg2,arg3)

  
#define debug(info) 
#define debug1(info,arg1) 
#define debug2(info,arg1,arg2) 
#define debug3(info,arg1,arg2,arg3) 

#define comefrom() printf("come from :%s\n",g_where_comein)
#define comein(info) {strcpy(g_where_comein,info);}
#define tile_block_index(row,col) ( ((row)%32 <16 ? 0:2 ) + ((col)%32 < 16 ? 0:1 ) )
//#define near_middle(pix) ((pix)%32 <10 ? 0 : ( ((pix)%32)<22 ? 1:2) )
#define near_middle(pix) ((pix)%32 >6 && (pix)%32 < 26 )

int game_random(int min,int max){
	return ( rand()%(max - min +1)) + min;
}

/*
	funtion:返回 x ms需要播放的帧数？？？
*/
int click(int ms){
	return (float)ms*g_fps/1000.;
}

void Game::delay(){
	usleep(1000000/g_fps );//帧与帧之间的播放时间间隔 这里的1000000是为了秒与微秒之间的单位转换
	return;
}

void Game::mark_time(){
	struct timeval tv;
	m_old_time = m_time;
	clock_gettime(CLOCK_REALTIME,&m_time);

}
void handle_event(HSL::Event&,void*);

void Game::init(){
	
	debug("in game::init \n");

	/*
		对于Server端而言，一开始就是splash状态——因为客户只有经过select状态之后才有可能连接网络
		一旦连接网络，就马上是从GameSplash开始
	*/
	set_state(GameStart);
	set_state(GameSelect);//necessary to set m_level = 1 as init condition
	set_state(GameSplash);

	/*
		sync将清除当前客户机仍未发送给X服务器的消息——发给服务器，兵器而等待X服务器结束处理这些请求

	*/
	m_win.sync(true);
	debug("out game::init \n");

}
void Game::update_screen(){

	m_win.draw(m_buffer);
	m_win.flush();

}
GameState Game::state(){
	return m_game_state;
}

void Game::set_state(GameState gs){
	//set_level 是不会反复调用的,适合初始化一个状态的开始数据
	//m_game_state 不能直接被更改，而是在每次改变时候都需要调用这个函数
	//因为state更改总有一系列的附加动作
	//set_state只作此状态开始的一系列工作,不做上一状态的扫尾工作,比如m_level++ 这是上一状态结束的动作
	m_game_state = gs;

	switch (gs){
		case GameSelect:
			//进入Selsect实际上意味着整个游戏全局数据的初始化
			//而对于Run状态和Splash状态始终是捆绑的,故实际上在全局数据看来,可以看成一个状态
			//进入Run的初始化,仅为单局数据的初始化,对于player_level m_level这些全局数据 不能在那里初始化
			m_level = 1;
			m_player_life_left[0]=2;
			m_player_life_left[1]=2;
			m_player_level[0]=0;
			m_player_level[1]=0;
			m_select_click = 0;
			break;
		case GameSplash:
			//splashing时候需要地图
			init_map();
			m_splash_click = 0;
			//消息显示不跨局,但在splash时候需要
			m_msg_click = 0;
		        m_splash_style = (SplashStyle)(game_random(0,3));
                        char str[100];
			sprintf(str,"Game start,Level %d ...",m_level);
			show_message(str,9000);
			break;

		case GameWin:
			//在Win状态中仅有一次将img_win画到buffer,故放在这里
			
			m_buffer.draw(m_img_win,0,0,640,480,0,0);
			m_win_click = 0;
			break;

		case GameRun:


			//驱动create_bore
			//这里不要改变player_level
			m_player_living[0]=false;
			m_player_living[1]=false;
			m_player_bullet_living[0]=0;
			m_player_bullet_living[1]=0;
			m_player_bore_living[0]=false;
			m_player_bore_living[1]=false;
			m_player_freezing = false;
		
			m_last_enemy_bore_click = 0;
		
			m_enemy_living = 0;
			m_enemy_level = 0;
			m_enemy_left = 20;
			m_enemy_freezing = false;
			m_enemy_bullet_living = 0;
			m_enemy_bore_living = 0;
	
	
			m_big_explode_living = 0;
			m_small_explode_living = 0;
			m_hawk_explode_living = false;
	
			m_bonus_living = false;
	
			//积分系统
			for(int i=0;i<2;i++)
				for(int j=0;j<4;j++)
					m_count[i][j]=0;

			m_win_delay_click = 0;
			m_over_delay_click = 0;
			break;
		case GameStart:
			m_start_click = 0;

			//第一次使用m_buffer,直接打开
			m_buffer.open("./img/logo.bmp");

		case GameOver:
			//退出Game Over 画面需要此值
			m_over_click = 0;
			break;

		default:
			//其他未发现需要设置数据
			break;
	}
}

/*
	HSL::Image
*/

//m_img_select_single.open("./img/select_single.bmp");
//m_img_select_double.open("./img/select_double.bmp");
//m_img_world.open("./img/run.bmp");
//m_img_win.open("./img/win.bmp");
//m_img_over.open("./img/over.bmp");


/*
	HSL::Font
*/

//m_pen.open("./hsl/font/font.bmp");

//Game::Game(){}
Game::Game(sPCLIENT_DATA pClient1_,sPCLIENT_DATA pClient2_):m_map()
{

	//初始化玩家指针

    if(pClient1_ !=NULL && pClient2_ != NULL){
	pClient1 = pClient1_;
	pClient1->player_type = 0;

	pClient2 = pClient2_;
	pClient2->player_type = 1;

	pClient1->in_Game = 1;
	pClient2->in_Game = 1;

	//sending onSendFriend
	onCache_dPACKET_FRIEND(pClient1, pClient2);
	//并且立即发出
	if (FlushSendBuff(pClient1)<0||FlushSendBuff(pClient2)<0) {
		log("[ERROR::BAD_DATA_TRANSFER_FRIEND]IN GAME \r\n\r\n\r\n");
		DisconnectClient(pClient1);
		DisconnectClient(pClient2);
		return;
	}
		

	//初始化互斥锁
	pthread_mutex_init(&m_mutex,NULL);  

	srand((int)time(NULL)%10000);

	//设置UI画面资源（开始游戏、进行游戏、游戏结束、字体等等）
	m_img_select_single.open("./img/select_single.bmp");
	m_img_select_double.open("./img/select_double.bmp");
	m_img_world.open("./img/run.bmp");
	m_img_win.open("./img/win.bmp");
	m_img_over.open("./img/over.bmp");
	m_pen.open("./hsl/font/font.bmp");


	/*m_audio.add_sound(SoundTankHitted,"./sound/playerhitted.wav");
	m_audio.add_sound(SoundPlayerGetBonus,"./sound/enemygetbonus.wav");
	m_audio.add_sound(SoundEnemyGetBonus,"./sound/enemygetbonus.wav");*/
//	m_audio.add_sound(SoundPlayerAttack,"./sound/playerattack.wav");
}
}

Game::~Game(){
//	pthread_mutex_lock(&m_mutex);
//	pthread_mutex_destroy(&m_mutex);
}


void Game::set_timer(int ms,TimerHandler timer_handler,void* parg){
	HSL::SetTimer(ms,timer_handler,parg);
}



void handle_timer(void* parg);


void Game::frameInfo(WORD& frame_num) {
	log("[INFO::FRAME_NUM]%d \r\n", frame_num);
	//首先封装首帧，每次循环发送一次帧的数据包，数据包由帧号数据包-数据组成

 	//onCache_FRAME_NO(frame_num, pClient1, pClient2);
	frame_num = (frame_num + 1) % maxFrameNum;//帧号从0开始
}

/*
		while(!GameEnd){
			1.处理事件
			2.根据游戏状态绘制相应界面	并且更新相应的click
			3.依据fps睡眠相应时间
*/
void Game::run(){
//	set_timer(1000/g_fps,handle_timer,(void*)(this));
	WORD frame_num = 0;
	while ( m_game_state != GameEnd ){
		
		if (pClient1 == NULL && pClient2 == NULL) {
			return;
		}

		/*
			try_to_recv_handle_user_order
		*/
		process_input(frame_num);

	/*
			function:
			根据当前的game的state来：
				1.绘制对应界面
					->call draw_screen_on_xxxstate()
				2.更新数据
					->call update_auto_data_on_xxxstate()
						->可能会发生游戏状态的改变
	*/
		//update and flush


//		if(m_game_state == GameSplash){
		update_auto_data(frame_num);

		//于此处发送帧
		

		/*
			睡眠时间，保证帧速率fps
		*/
		delay();
	}

}


void Game::try_send() {

	fd_set write_set;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*
	fd_set init
	*/
	FD_ZERO(&write_set);

	/*
	fd_set bind
	*/
	if (pClient1 != NULL) {
		FD_SET(pClient1->m_scok, &write_set);
	}
	if (pClient2 != NULL) {
		FD_SET(pClient2->m_scok, &write_set);
	}

	/*
	GET NFDS
	*/
	SOCKET nfds;
	if (pClient1 != NULL && pClient2 != NULL) {
		nfds = pClient1->m_scok > pClient2->m_scok ? pClient1->m_scok : pClient2->m_scok;
	}
	else if (pClient1 != NULL)
	{
		nfds = pClient1->m_scok;
	}
	else if (pClient2 != NULL) {
		nfds = pClient2->m_scok;
	}
	else {
		log("[ERROR::DISCONNECT_BOTH]\r\n\r\n\r\n");
		return;
	}


	/*
		SELECT
	*/
	if (select(nfds + 1, NULL, &write_set, NULL, &tv) < -1) {
		log("[Error::select::subThread]\r\n");
		return;
	}

	/*
		send
	*/
	//client1
	if (pClient1 != NULL && pClient1->m_sendSize && FD_ISSET(pClient1->m_scok, &write_set)) {
		if (FlushSendBuff(pClient1) < 0) {
			log("[ERROR::DISCONNECT_CLIENT1_BAD_SEND]%s in try_send \r\n\r\n\r\n", pClient1->m_IP);
			DisconnectClient(pClient1);
		}
	}
	//client2
	if (pClient2 != NULL && pClient2->m_sendSize && FD_ISSET(pClient2->m_scok, &write_set)) {
	if (FlushSendBuff(pClient2) < 0) {
			log("[ERROR::DISCONNECT_CLIENT2_BAD_SEND]%s in try_send \r\n\r\n\r\n", pClient2->m_IP);
			DisconnectClient(pClient2);
		}
	}
	
	if(pClient1 == NULL && pClient2 == NULL){
		set_state(GameEnd);
	}
}


void Game::recv_handle() {

//test
	log("debug info start to recv_handle\r\n");

	if(pClient1 == NULL && pClient2 == NULL){
		return;
	}
	

//获取客户socket传送过来的数据并进行对应的处理
	fd_set read_set;
	fd_set exc_set;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*
		fd_set init
	*/
	FD_ZERO(&read_set);
	FD_ZERO(&exc_set);


        log("debug info start to bind \r\n");



	/*
		fd_set bind
	*/
	if (pClient1 != NULL) {
		FD_SET(pClient1->m_scok, &read_set);
		FD_SET(pClient1->m_scok, &exc_set);
	}
	if (pClient2 != NULL) {
		FD_SET(pClient2->m_scok, &read_set);
		FD_SET(pClient2->m_scok, &exc_set);
	}

	/*
		GET NFDS
	*/
	SOCKET nfds;
	if (pClient1 != NULL && pClient2 != NULL) {
		nfds = pClient1->m_scok > pClient2->m_scok ? pClient1->m_scok : pClient2->m_scok;
	}
	else if (pClient1 != NULL)
	{
		nfds = pClient1->m_scok;
	}
	else if (pClient2 != NULL) {
		nfds = pClient2->m_scok;
	}
	else {
		log("[ERROR::DISCONNECT_BOTH]\r\n\r\n\r\n");
		return;
	}

        log("debug info got  nfds:%d\r\n",nfds);





	/*
		 SELECT
	*/
	if (select(nfds + 1, &read_set, NULL,&exc_set, &tv) < -1) {
		log("[Error::select::subThread]\r\n");
		return;
	}


        log("debug info strat to check\r\n");


	/*
		CHECK
	*/
	if (pClient1 != NULL && pClient2 != NULL && (FD_ISSET(pClient1->m_scok, &exc_set) || FD_ISSET(pClient2->m_scok, &exc_set))) {
		log("[ERROR::DISCONNECT_BOTH]%s ; %s IN recv_handle\r\n\r\n\r\n", pClient1->m_IP,pClient2->m_IP);
		DisconnectClient(pClient1);
		DisconnectClient(pClient2);
	}
	else if (pClient1 != NULL && FD_ISSET(pClient1->m_scok, &exc_set)) {
		log("[ERROR::DISCONNECT_CLIENT1]%s IN recv_handle\r\n\r\n\r\n", pClient1->m_IP);
		DisconnectClient(pClient1);
	}
	else if (pClient2 != NULL && FD_ISSET(pClient2->m_scok, &exc_set)) {
		log("[ERROR::DISCONNECT_CLIENT2]%s IN recv_handle\r\n\r\n\r\n", pClient2->m_IP);
		DisconnectClient(pClient2);
	}


        log("debug info end check;start recv \r\n");



	/*
		try to recv
	*/
	//client1
	if (pClient1 != NULL && FD_ISSET(pClient1->m_scok, &read_set)) {
		if (!RecvFromClient(pClient1)) {
			log("[ERROR::DISCONNECT_UNKNOWN_SOURCE] %s IN recv_handle\r\n\r\n\r\n", pClient1->m_IP);
			DisconnectClient(pClient1);
		}

        log("debug info recv successfully from client1 \r\n");
	}

	//client2
	if (pClient2 != NULL && FD_ISSET(pClient2->m_scok, &read_set)) {
		if (!RecvFromClient(pClient2)) {
			log("[ERROR::DISCONNECT_UNKNOWN_SOURCE] %s IN recv_handle\r\n\r\n\r\n", pClient2->m_IP);
			DisconnectClient(pClient2);
		}
        log("debug info recv successfully from client2\r\n");


	}



        log("debug info:recv end ;strat to handle \r\n");

	/*
		handle all recv
	*/
	//client1
	if (pClient1 != NULL && pClient1->m_recvSize) {

        log("debug info:ready to call CENTER \r\n");


		if (!onHandle_CONTROL_CENTER( *this,pClient1,pClient2))
		{
			log("[ERROR::DISCONNECT_BAD_HANDLE] %s IN recv_handle\r\n\r\n\r\n", pClient1->m_IP);
			DisconnectClient(pClient1);
		}
        log("debug info end of call CENTER without error\r\n");

	}

	//client2
	if (pClient2 != NULL && pClient2->m_recvSize) {

        log("debug info:ready to call CENTER client2 \r\n");


		if (!onHandle_CONTROL_CENTER(*this, pClient2,pClient1))
		{
			log("[ERROR::DISCONNECT_BAD_HANDLE] %s IN recv_handle\r\n\r\n\r\n", pClient2->m_IP);
			DisconnectClient(pClient2);
		}
        log("debug info end of call CENTER without error\r\n");


	}
	
}

/*
	处理当前发生的所有事件
*/
void Game::process_input(WORD& frame_no){
	//while(m_win.pending()){

	//	/*
	//		获取当前发生的事件
	//			-> set m_event
	//	*/
	//	m_win.next_event();

	//	/*
	//		call the rightNow handler to handle the event
	//	*/
	//	m_win.handle_event();
	//}
	switch (m_game_state)
	{
	case GameRun:
		//frameInfo(frame_no);
		recv_handle();
	//当处于其他状态的时候，不尝试接收数据包
		//GameSplash和GameWin都不提供按键直接跳过的功能，为了简单化，实现服务器端和客户端的同步
		//客户端应当解除这几种状态的按键跳过功能
	default:
		break;
	}
}

/*
	function:将window的event_handler设置，并且配置其参数列表void * m_parg
	所谓的EventHandler事实上是一个函数指针
*/
void Game::set_event_handler(HSL::EventHandler event_handler,void* parg){
	m_win.set_event_handler(event_handler,parg);
}



//handle_event只负责对必要数据的修改工作
void game_control_on_run(HSL::Event& , Game& );


/*
	事件处理函数：
		1.KeyPress事件
			针对不同的游戏状态
				针对不同的按键	
					作出相应的响应
		2.DestroyNotify事件
			置游戏状态为GameEnd
*/
void handle_event(HSL::Event& event,void* arg){
	Game* pgame = (Game*)arg;

	//锁住游戏,虽然这会严重降低系统性能
//	pgame->lock_game();
	switch (event.type){
		case KeyPress:
			switch (pgame->state()){
				case GameRun:
					//run
					//respond of running game ,change game data;
					game_control_on_run(event,*pgame);
					break;
			/*	case GameSelect:
					static bool single = false;
					switch (HSL::KeyValue(event)){
						case KEY_Up:
						case KEY_Down:
						case KEY_w:
						case KEY_s:
							single = !single;
							pgame->set_single_player(single);
							break;
						case KEY_Return:
						case KEY_j:
							pgame->set_state(GameSplash);
							break;
						case KEY_Esc:
							pgame->set_state(GameEnd);
						default:
							break;
					}
					break;*/
					//以下这些都是一些过渡态,只要有按键就可以打断

					/*
						过渡态，捕捉到按键就直接从GameSplash状态变成GameRun状态
					*/
				case GameSplash:
					pgame->set_state(GameRun);
					break;
				case GameOver:    //show game over

					/*
						过渡态，捕捉到按键就直接从GameStart状态变成GameSelect状态
					*/
				case GameStart:   //show hah name
					pgame->set_state(GameSelect);
					break;
				default:
					break;
			}
			break;
		case DestroyNotify://猜测：是关闭窗口的时候触发
			pgame->set_state(GameEnd);
			break;
		default:
			//仅仅关心按键
			break;
	}
//	pgame->unlock_game();

}


/*
	根据按键，作出相应改变：
		方向改变[if diff]、位置改变[if same]
		射击[if j or J or space and confirm to the amount limit on bullet ]
*/
void game_control_on_run(HSL::Event& event,Game& game){
	switch( HSL::KeyValue(event)){
		case KEY_w:
		lab_KEY_w:
			//表示按下的是W或者是"w（猜测）"
			if(game.player_freezing())break;
			if(game.player_dir(0) == DirUp){
				game.player_move(0);
			}else{
				game.set_player_dir(0,DirUp);
			}
			break;
		case KEY_s:
			lab_KEY_s:
			if(game.player_freezing())break;
			if(game.player_dir(0) == DirDown){
				game.player_move(0);
			}else{
				game.set_player_dir(0,DirDown);
			}
			break;
		case KEY_a:
			lab_KEY_a:
			if(game.player_freezing())break;
			if(game.player_dir(0) == DirLeft){
				game.player_move(0);
			}else{
				game.set_player_dir(0,DirLeft);
			}
			break;
		case KEY_d:
			lab_KEY_d:
			if(game.player_freezing())break;
			if(game.player_dir(0) == DirRight){
				game.player_move(0);
			}else{
				game.set_player_dir(0,DirRight);
			}
			break;
		case KEY_j:
		lab_KEY_j:
			if(game.player_freezing())break;
			game.player_try_attack(0);
			break;

			//player2
		case KEY_Up:// up arrow
			if(game.player_freezing())break;
			if(!game.single_player()){
				if(game.player_dir(1) == DirUp){
					game.player_move(1);
				}else{
					game.set_player_dir(1,DirUp);
				}
			}else goto lab_KEY_w;
			break;
		case KEY_Down:
			if(game.player_freezing())break;
			if(!game.single_player()){
				if(game.player_dir(1) == DirDown){
					game.player_move(1);
				}else{
					game.set_player_dir(1,DirDown);
				}
			}else goto lab_KEY_s;
			break;
		case KEY_Left:
			if(game.player_freezing())break;
			if(!game.single_player()){
				if(game.player_dir(1) == DirLeft){
					game.player_move(1);
				}else{
					game.set_player_dir(1,DirLeft);
				}
			}else goto lab_KEY_a;
			break;
		case KEY_Right:
			if(game.player_freezing())break;
			if(!game.single_player()){
				if(game.player_dir(1) == DirRight){
					game.player_move(1);
				}else{
					game.set_player_dir(1,DirRight);
				}
			}else goto lab_KEY_d;
			break;
		case KEY_Space:
			if(game.player_freezing())break;
			if(!game.single_player()){
				//单人模式下J和空格都可以发射子弹
				game.player_try_attack(1);
			}else goto lab_KEY_j;
			break;
			//终止当前局游戏
		case KEY_Esc:
			game.set_state(GameSelect);
			break;
		default:
			break;

	}
}


void Game::player_try_attack(int i){
	if(    m_player_living[i]&&
		   m_player_bullet_living[i] <= ((int)(m_player[i].level()>TankLevel1) + 1) && 
		   !m_player_freezing &&
		   m_player[i].state() != PlayerWink ){
		create_player_bullet(i);
	}
}
		   
			

Direction Game::player_dir(int i){
	return m_player[i].dir();
}

void Game::set_player_dir(int i,Direction d){
	m_player[i].set_dir(d);
}

void Game::player_move(int i){
//player 对方向键的相应
//能进则进 不能则停
//不再考虑freezing的问题,这个函数只考虑外因(外界是否制约Tank  Move,不考虑Tank是否有能力Move)

	int begin_row,begin_col;
	int speed;
	int width,height;
	BlockType bt;

	begin_row = m_player[i].row()-m_player[i].hot_row();
	begin_col = m_player[i].col()-m_player[i].hot_col();
	speed = m_player[i].speed();
	width = m_player[i].width();
	height = m_player[i].height();




	switch (m_player[i].dir()){
		case DirUp:
			//1:是否出界
			if(begin_row - speed < 0 )return ;
			//2.是否地图可达
			//从角点采样2+1个点 因为地图最小块是16  
			bt = m_map.pixel_type(begin_row - speed,begin_col);
			if(bt != BlockNull && bt != BlockTree )return ;			
			bt = m_map.pixel_type(begin_row - speed,begin_col + 16);
			if(bt != BlockNull && bt != BlockTree )return ;			
			bt = m_map.pixel_type(begin_row - speed,begin_col + width);
			if(bt != BlockNull && bt != BlockTree )return ;

			//3.是否被其他车辆阻塞
			//a.敌人
			m_player[i].add_row(-1*speed);
			for(int k=0;k<m_enemy_living;k++){
				if(m_enemy[k].check_hit(m_player[i])){
					m_player[i].add_row(speed);
					return ;
				}
			}
			//b.其他玩家
			if(!m_single_player && m_player[!i].check_hit(m_player[i])){
				m_player[i].add_row(speed);
				return ;
			}

			//c. enemy bore
			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_player[i].check_hit(m_enemy_bore[j])){
					m_player[i].add_row(speed);
					return ;
				}
			}
			//player bore
			//no need
			return ;

		case DirRight:
			if(begin_col + width + speed  > m_map.pixel_width() )return;

			bt = m_map.pixel_type(begin_row , begin_col+width+speed);
			if(bt != BlockNull && bt != BlockTree)return ;
			bt = m_map.pixel_type(begin_row + 16, begin_col+width+speed);
			if(bt != BlockNull && bt != BlockTree)return ;
			bt = m_map.pixel_type(begin_row +height,begin_col +width+ speed); 
			if(bt != BlockNull && bt != BlockTree)return ;

			m_player[i].add_col(speed);
			for(int k=0;k<m_enemy_living;k++){
				if(m_enemy[k].check_hit(m_player[i])){
					m_player[i].add_col(-speed);
					return ;
				}
			}
			if(!m_single_player && m_player[!i].check_hit(m_player[i])){
				m_player[i].add_col(-speed);
				return ;
			}


			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_player[i].check_hit(m_enemy_bore[j])){
					m_player[i].add_col(-speed);
					return ;
				}
			}

			if(!m_single_player && m_player_bore_living[!i]){
				if(m_player[i].check_hit(m_player_bore[!i])){
					m_player[i].add_col(-speed);
					return;
				}
			}

			return ;
		case DirDown:
			if(begin_row + height + speed  > m_map.pixel_height() )return ;

			bt = m_map.pixel_type(begin_row + height + speed,begin_col );
			if(bt != BlockNull && bt != BlockTree )return ;			
			bt = m_map.pixel_type(begin_row + height + speed,begin_col + 16);
			if(bt != BlockNull && bt != BlockTree )return ;			
			bt = m_map.pixel_type(begin_row + height + speed,begin_col + width );
			if(bt != BlockNull && bt != BlockTree)return ;

			m_player[i].add_row(speed);
			for(int k=0;k<m_enemy_living;k++){
				if(m_enemy[k].check_hit(m_player[i])){
					m_player[i].add_row(-speed);
					return ;
				}
			}
			if(!m_single_player && m_player[!i].check_hit(m_player[i])){
				m_player[i].add_row(-speed);
				return ;
			}
			if(!m_single_player && m_player_bore_living[!i]){
				if(m_player[i].check_hit(m_player_bore[!i])){
					m_player[i].add_row(-speed);
					return;
				}
			}
			return ;

		case DirLeft:
			if(begin_col - speed  < 0 )return ;
			bt = m_map.pixel_type(begin_row , begin_col-speed);
			if(bt != BlockNull && bt != BlockTree)return ;
			bt = m_map.pixel_type(begin_row + 16, begin_col-speed);
			if(bt != BlockNull && bt != BlockTree)return ;
			bt = m_map.pixel_type(begin_row + height ,begin_col-speed); 
			if(bt != BlockNull && bt != BlockTree)return ;

			m_player[i].add_col(-speed);
			for(int k=0;k<m_enemy_living;k++){
				if(m_enemy[k].check_hit(m_player[i])){
					m_player[i].add_col(speed);
					return ;
				}
			}
			if(!m_single_player && m_player[!i].check_hit(m_player[i])){
				m_player[i].add_col(speed);
				return ;
			}
			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_player[i].check_hit(m_enemy_bore[j])){
					m_player[i].add_col(speed);
					return ;
				}
			}
			if(!m_single_player && m_player_bore_living[!i]){
				if(m_player[i].check_hit(m_player_bore[!i])){
					m_player[i].add_col(speed);
					return;
				}

			}
			return ;

		default:
			return ;
	}
}


bool Game::single_player(){
	return m_single_player;
}
bool Game::player_freezing(){
	return m_player_freezing;
}
void handle_timer(void* parg){
	Game* pg = (Game*)parg;
	//pg->update_auto_data();
//	if(pg->try_lock_game()){
		//pg->update_screen();
//		pg->unlock_game();
//	}else{
//		printf("lock game fail ...\n");
//	}
//	pg->lock_game();
//	pg->update_auto_data();
//	pg->unlock_game();
}

/*
	function:
	根据当前的game的state来：
		1.绘制对应界面 
			->call draw_screen_on_xxxstate()
		2.更新数据
			->call update_auto_data_on_xxxstate()
				->可能会发生游戏状态的改变
*/
void Game::update_auto_data(WORD& frame_num){

	switch(m_game_state){
		case GameRun:
			/*
				state : run

			*/
			draw_screen_on_run();//纯粹绘制界面相关
			/*
				state change:
					->GameWin / GameOver
			*/
			update_auto_data_on_run();
	
				
			log("debug info::end update data on run ;start cache GAME_RUN_FRAME\r\n");
		        onCache_GAME_RUN_FRAME(*this,pClient1,pClient2,frame_num);
			log("debug info::end cache GAME_RUN_FRAME;start try send\r\n");


        		frame_num = (frame_num + 1) % maxFrameNum;//帧号从0开始
	
			try_send();
			
			break;

		case GameOver:
			//debug("in on_over switch...\n");
			draw_screen_on_over();

			/*
					0   - 1.5 sec  : show GameOver gradually
					1.5 - 2.5 sec  : still
					2.5 - 4.5 sec  : 隐去GameOver
					>4.5      sec  : 置状态GameSelect
			*/
	        	log("debug info :: try cache GAMEOVER \r\n");
			onCache_GAME_OVER_FRAME(*this,pClient1,pClient2,frame_num);
	        	frame_num = (frame_num + 1) % maxFrameNum;//帧号从0开始

			try_send();
			usleep(4000000);
			m_game_state = GameEnd;
			//disconnect here???
			update_auto_data_on_over();
			break;



			/*
				游戏状态会从GameSelect转成Splash，即进入游戏时的特效制作时间
			*/
		case GameSplash:
			draw_screen_on_splash();

			/*
				click:
					->m_splash_click

				setState[if m_splash_click > 32]
					->GameRun
			*/
			update_auto_data_on_splash();
			break;

		case GameWin:
			draw_screen_on_win();

			/*
				1.逐渐绘制本关通关的界面
				
				2.setState[if m_win_click = click(7500)]
					->GameSplash
			*/
			onCache_GAME_WIN_FRAME(*this,pClient1,pClient2,frame_num);
		        frame_num = (frame_num + 1) % maxFrameNum;//帧号从0开始

			try_send();
			update_auto_data_on_win();
			break;
		default:
			break;
	}

}

/*
	m_start_click:
		0sec - 1.5 sec :显示作者信息
		1.5sec - 3.5 sec: 渐变成黑色（每次*0.92,最终被转换成了0x00000000）
		>3.5 sec 设置状态为GameSelect
*/
void Game::update_auto_data_on_start(){

	/*
		显示作者信息
	*/
	m_start_click ++;
	if(m_start_click < click(1.5*1000))return;
	if(m_start_click >= click(3.5*1000))
		set_state(GameSelect);



	/*
		
	*/
	HSL::Color tmp_co;
	HSL::Color co;
	for (int i= 0;i<m_buffer.height();i++)
		for (int j= 0;j<m_buffer.width();j++){
			tmp_co = m_buffer.pixel(i,j);
			co = RGB(RGB_R(tmp_co)*.92,RGB_G(tmp_co)*.92,RGB_B(tmp_co)*.92);
			m_buffer.set_pixel(co,i,j);
		}


}
void Game::draw_screen_on_select(){
	//显示单人or双人
	
	if(m_single_player){
		m_win.draw(m_img_select_single,0,0,
				 m_img_select_single.width(),
				 m_img_select_single.height(),
				 0,0);

	}else{
		m_win.draw(m_img_select_double,0,0,
				 m_img_select_double.width(),
				 m_img_select_double.height(),
				 0,0);

	}
}

/*
	click:
		->m_splash_click 

	setState[if m_splash_click > 32]
		->GameRun
*/
void Game::update_auto_data_on_splash(){
	m_splash_click ++;
	deal_with_message();
	
	if(m_splash_click > 32 ){//}click(1.5*1000)){
		set_state(GameRun);
	}
}


void Game::draw_screen_on_splash(){
	//根据m_splash_style 和 m_splash_click两个参数进行显示
	//每次必须重画,因为Splash过程会弄脏world
	m_buffer.draw(m_img_world,0,0,640,480,0,0);
	HSL::Color co=RGB(100,100,100);
	switch (m_splash_style){
		case SplashJalousie:
			//百叶窗
			//
			debug("in splash Jalousie...\n");
			debug1("splash click =%d...\n",m_splash_click);
			m_map.blit_tile(m_buffer,row_off,col_off);
			for(int cou=0;cou<8;cou++)
				for(int row=0;row<32*13;row++)
					for(int col=0;col<4*13*(1.-m_splash_click/32.);col++){
						m_buffer.set_pixel(co,row+row_off,col+cou*4*13+col_off);
					}
			break;
		case SplashWindow:
			debug("in splash window...\n");

			m_map.blit_tile(m_buffer,row_off,col_off);
			for(int row=0;row<32*13;row++)
				for(int col=0;col<(int)(32*13/2)*(1. - m_splash_click/32.);col++){
					m_buffer.set_pixel(co,row+row_off,col+col_off);
					m_buffer.set_pixel(co,row+row_off,32*13-col+col_off);
				}
			break;
		case SplashBlock:
			m_map.blit_tile(m_buffer,row_off,col_off);
			for(int i=0;i<8;i++)
				for(int j=0;j<8;j++)
					for(int row=0;row<13*4;row++)
						for(int col=0;col<13*4;col++)
							if((row+col)<13*4*2*(1-m_splash_click/32.))
								m_buffer.set_pixel(co,i*13*4+row+row_off,j*13*4+col+col_off);
			break;
		default:
			m_map.blit_tile(m_buffer,row_off,col_off);
			for(int i=0;i<8;i++)
				for(int j=0;j<8;j++)
					for(int row=0;row<13*4;row++)
						for(int col=0;col<13*4;col++)
							if(row>13*4*m_splash_click/32. || col>13*4*m_splash_click/32.)
								m_buffer.set_pixel(co,i*13*4+row+row_off,j*13*4+col+col_off);

			break;
	}

	//有消息就显示消息
	if(m_msg_click >0)
		m_pen.put_str(m_buffer,m_msg.c_str(),450,64);
	update_screen();

}
/*
	逐渐绘制游戏胜利结束画面：
	0.5 s : 绘制游戏结束时候的等级
	1	s : 绘制玩家剩余的生命值【考虑了双人模式——下同】
	1.5 s : 绘制玩家的等级
	2   s : 绘制m_count[i][1]存储的值——击败某种坦克的数量，并计算得分
	2.5 s : 绘制m_count[i][2]存储的值——击败某种坦克的数量，并计算得分
	3   s : 绘制m_count[i][3]存储的值——击败某种坦克的数量，并计算得分
	3.5 s : 绘制m_count[i][0]存储的值——击败某种坦克的数量，并计算得分
	4   s : 绘制最终总得分
	6   s : 绘制各玩家新增的生命（最终得分/300）
	7.5 s : 递增游戏等级，将游戏状态置为GameSplash

*/
void Game::update_auto_data_on_win(){
	debug("in update auto data on win\n");
	char str[100];
	m_win_click ++;

	for(int i=0;i<=(int)!m_single_player;i++){

		if(m_win_click == click(500)){
			sprintf(str,"%d",m_level);
			debug("sprintf(str,%d,m_level);");
			m_pen.put_str(m_buffer,str,21,379);
			break;
		}else if(m_win_click == click(1000)){
			sprintf(str,"%d",m_player_life_left[i]);
			m_pen.put_str(m_buffer,str,130,150+i*(473-150));
		}else if(m_win_click == click(1500)){
			sprintf(str,"%d",(int)m_player[i].level());
			m_pen.put_str(m_buffer,str,169,150+i*(473-150));

		}else if(m_win_click == click(2000)){
			sprintf(str,"%d",m_count[i][1]);
			m_pen.put_str(m_buffer,str,220,160 + i*(475-160));
			sprintf(str,"%d",m_count[i][1]*10);
			m_pen.put_str(m_buffer,str,220,230 + i*(545-230));
		}else if(m_win_click == click(2500)){
			sprintf(str,"%d",m_count[i][2]);
			m_pen.put_str(m_buffer,str,270,160 + i*(475-160));
			sprintf(str,"%d",m_count[i][2]*20);
			m_pen.put_str(m_buffer,str,270,230 + i*(545-230));
		}else if(m_win_click == click(3000)){
			sprintf(str,"%d",m_count[i][3]);
			m_pen.put_str(m_buffer,str,317,160 + i*(475-160));
			sprintf(str,"%d",m_count[i][3]*50);
			m_pen.put_str(m_buffer,str,317,230 + i*(545-230));
		}else if(m_win_click == click(3500)){
			sprintf(str,"%d",m_count[i][0]);
			m_pen.put_str(m_buffer,str,371,160 + i*(475-160));
			sprintf(str,"%d",m_count[i][0]*50);
			m_pen.put_str(m_buffer,str,371,230 + i*(545-230));
		}else if(m_win_click == click(4000)){
			m_score[i] = m_count[i][0]*50+
				         m_count[i][1]*10+
						 m_count[i][2]*20+
						 m_count[i][3]*50;
			sprintf(str,"%d",m_score[i]);
			m_pen.put_str(m_buffer,str,428,185+i*(505-195));
		}else if(m_win_click == click(6000)){
			if(m_score[i] > 300){

				for(int row=425;row <458;row++)
					for(int w =0;w<177;w++)
						m_buffer.set_pixel(RGB(0,0,0),row,63+w+i*(381-63));

				m_player_life_left[i] += m_score[i]/300;
				sprintf(str,"Player%d add  %d life !!!",i,m_score[i]/300);
				m_pen.put_str(m_buffer,str,430,20+i*(331-20));
			}
		}else if(m_win_click == click(7500)){
			m_level ++;
			set_state(GameSplash);
			break;
		}
	}

	debug("out update auto data on win\n");
}
void Game::draw_screen_on_win(){
	update_screen();

}


void Game::update_auto_data_on_over(){
	//游戏over的前提：两个玩家都失败或者老鹰挂掉
	//此时客户端将重新进入GameSelect状态，服务器端应该断开两者连接
	if (pClient1 != NULL) {
		log("[INFO::GAMEOVER]%s in update_auto_data_on_over\r\n",pClient1->m_IP);
		DisconnectClient(pClient1);
	}

	if (pClient2 != NULL) {
		log("[INFO::GAMEOVER]%s in update_auto_data_on_over\r\n",pClient2->m_IP);
		DisconnectClient(pClient1);
	}

	//置游戏状态为GameEnd来退出run循环
	set_state(GameEnd);
}

void Game::draw_screen_on_over(){
	debug ("in draw screen on over \n");
	//全画,因为GameOver有变暗环节
	m_win.draw(m_buffer,
			   0,0,
			   m_buffer.width(),
			   m_buffer.height(),
			   0,0);
	debug ("out draw screen on over \n");
}

void Game::update_auto_data_on_run()
{

	debug("\n");
	debug("\n");
	debug("\n");
	debug1("当前为%d个player \n",(int)!m_single_player +1);
	debug1("m_enemy_living = %d\n",m_enemy_living);
	debug1("m_enemy_left = %d \n",m_enemy_left);
	debug1("m_player_living0 = %d \n",(int)m_player_living[0]);
	debug1("m_player_living1 = %d \n",(int)m_player_living[1]);
	debug1("m_enemy_bore_living = %d \n",m_enemy_bore_living);
	debug1("m_player_bore_living0 = %d \n",(int)m_player_bore_living[0]);
	debug1("m_player_bore_living1 = %d \n",(int)m_player_bore_living[1]);
	debug1("m_enemy_bullet_living = %d \n",m_enemy_bullet_living);
	debug1("m_player0_bullet_living = %d \n",m_player_bullet_living[0]);
	debug1("m_player1_bullet_living = %d \n",m_player_bullet_living[1]);
	debug1("m_big_explode_living = %d \n",m_big_explode_living);
	debug1("m_small_explode_living = %d \n",m_small_explode_living);
	fflush(stdout);

	

	debug("===================\n");
	
	//test
	log("before map update\r\n");
	
	m_map.update();//不发送命令，因为在服务器端，该函数与deal_with_enemy_bullet绑在一起

	log("after mao update\r\n");




	log("debug info::start deal_en_bullet\r\n");

	/*
		处理敌军子弹：
		检测碰撞：
			-> 与fence相撞
			-> 与player相撞
			-> 与tile相撞
		
		更新子弹的位置：call move()

	*/
	deal_with_enemy_bullet();


	log("debug info::end deal_en_bullet;start deal_pl_bullet\r\n");
 
	/*
		处理我方子弹：
		检测碰撞：
			->与player相撞
			->与敌军tank相撞
			->与敌军子弹相撞
			->与player相撞
			->与fence相撞
			->与tile相撞
		更新子弹的位置：call move()
	*/
	deal_with_player_bullet();

	log("debug info::end deal_pl_bullet;start deal_pl\r\n");
	/*
		对四种player状态和游戏状态检查，并设置click
			1.游戏失败结束
			2.PlayerWink状态
			3.shield状态
			4.freezing状态
	*/
	deal_with_player(); //暂定由客户端全权代理


	log("debug info::end deal_pl;start deal_en\r\n");
	//onCache_CALL_DEAL_WITH_PLAYER(pClient1, pClient2);

	/*
		1.对游戏运行状态进行检查，并设置click
			->游戏胜利结束 return here
			->m_enemy_freezing
		2.对于每个存活的敌军坦克
			->尝试移动（不确定是否移动了）
			->尝试射击[if no freezing]
			->update click
					update click conditionly:
					->m_shield_click[if m_shield]
					->m_last_attack_click

	*/
	deal_with_enemy();//暂定由客户端全权代理
	

	log("debug info::end deal_en;start deal_pl_bore\r\n");

	/*
		处理player_bore
		判断当前是否需要、是否可以生成新的player
		if so ,then bore
	*/
	deal_with_player_bore();//DEAL_WITH_PLAYER_BORE

	log("debug info::end deal_pl_bore;start deal_en_bore\r\n");
	/*
		处理enemy_bore
	*/
	deal_with_enemy_bore();//CREATE_ENEMY_BORE BACK_OFF_CREATE_ENEMY_BORE CREATE_ENEMY REMOVE_ENEMY_BORE_I


	log("debug info::end deal_en_bore;start deal_bonus\r\n");
	/*
	if no bonus now then return
	else
	->update Bonus::m_last_click[if time out then set it false]
	->判断是否有敌人吃到了bonus，如果是，执行对应动作并return
	->判断是否有player吃到了bonus，如果是，执行对应的动作并return
	*/
	deal_with_bonus(); //客户端自行处理

	log("debug info::end deal_bonus;start deal_explode\r\n");
	/*
	deal with:
	->所有小型爆炸[click]
	->所有大型爆炸[click]
	->鹰爆炸[if living ][click]
	*/

	deal_with_explode();//客户端自行处理

	/*
		if m_msg_click >0
			then 递减m_msg_click
	*/


	log("debug info::end deal_explode;start deal_message\r\n");
	deal_with_message();//客户端自行处理

	//onCache_CALL_DEAL_WITH_BONUS_EXPLODE_MESSAGE_DELAY(pClient1,pClient2);


	/*
	click:
	->m_win_delay_click
	->m_over_delay_click

	logic:
	如果以上的click有被置成非0值，那么就会在这里被执行递增
	直到某个界限，分别设置游戏状态为GameWin或者GameOver
	*/

	log("debug info::end deal_message;start deal_delay\r\n");
	deal_with_delay();   //客户端自行处理

	debug("===================\n");
	
	log("debug info::done deal_delay\r\n");

}

/*
	敌军坦克i尝试射击
		-> if attack
			重置该敌军坦克的m_last_attack_click
			床架enemyBullet
*/

void Game::enemy_try_attack(int i){
//	return;
	if(m_enemy[i].attack_click()>click(1000*2) && !m_enemy_freezing){
		m_enemy[i].set_attack_click(0);
		create_enemy_bullet(i);
	}

}

/*
	只判断，并不移动
*/
bool Game::enemy_try_go_ahead(int i){


	//仔细检查逻辑

	int begin_row,begin_col,speed,width,height;
	begin_row = m_enemy[i].row()-m_enemy[i].hot_row();
	begin_col = m_enemy[i].col()-m_enemy[i].hot_col();
	speed = m_enemy[i].speed();
	width = m_enemy[i].width();
	height = m_enemy[i].height();
	BlockType bt;



	switch (m_enemy[i].dir()){
		case DirUp:
			//1.出界
			if(begin_row - speed < 0 )return false;

			//2.地图允许
			bt = m_map.pixel_type(begin_row - speed,begin_col);
			if(bt != BlockNull && bt != BlockTree )return false;			
			bt = m_map.pixel_type(begin_row - speed,begin_col + 16);
			if(bt != BlockNull && bt != BlockTree )return false;			
			bt = m_map.pixel_type(begin_row - speed,begin_col + width);
			if(bt != BlockNull && bt != BlockTree)return false;

			//3.其他车辆
			//a.enemy
			m_enemy[i].add_row(-speed);
			for(int k=0;k<m_enemy_living;k++){
				if(k == i)continue;
				if(m_enemy[k].check_hit(m_enemy[i])){
					m_enemy[i].add_row(speed);
					return false;
				}
			}
			//b.player
			for(int k=0;k<=!m_single_player;k++){
				if(m_enemy[i].check_hit(m_player[k])){
					m_enemy[i].add_row(speed);
					return false;
				}
			}

			//4.bore
			//a.enemy
			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_enemy[i].check_hit(m_enemy_bore[j])){
					m_enemy[i].add_row(speed);
					return false;
				}
			}

			//b.player no need
			return true;

		case DirRight:
			if(begin_col + width + speed  > m_map.pixel_width() )return false;
			bt = m_map.pixel_type(begin_row , begin_col+width+speed);
			if(bt != BlockNull && bt != BlockTree)return false;
			bt = m_map.pixel_type(begin_row + 16, begin_col+width+speed);
			if(bt != BlockNull && bt != BlockTree)return false;
			bt = m_map.pixel_type(begin_row + height,begin_col + width + speed); 
			if(bt != BlockNull && bt != BlockTree)return false;

			m_enemy[i].add_col(speed);
			for(int k=0;k<m_enemy_living;k++){
				if(k == i)continue;
				if(m_enemy[k].check_hit(m_enemy[i])){
					m_enemy[i].add_col(-speed);
					return false;
				}
			}
			for(int k=0;k<=!m_single_player;k++){
				if(m_enemy[i].check_hit(m_player[k])){
					m_enemy[i].add_col(-speed);
					return false;
				}
			}
			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_enemy[i].check_hit(m_enemy_bore[j])){
					m_enemy[i].add_col(-speed);
					return false;
				}
			}
			for(int j=0;j<=(int)!m_single_player;j++){
				if(m_player_bore_living[j]){
					if(m_enemy[i].check_hit(m_player_bore[j])){
						m_enemy[i].add_col(-speed);
						return false;
					}
				}
			}

			return true;

		case DirDown:
			if(begin_row + height + speed  > m_map.pixel_height() )return false;

			bt = m_map.pixel_type(begin_row + height + speed,begin_col);
			if(bt != BlockNull && bt != BlockTree )return false;			
			bt = m_map.pixel_type(begin_row + height + speed,begin_col+16);
			if(bt != BlockNull && bt != BlockTree )return false;			
			bt = m_map.pixel_type(begin_row +height +  speed,begin_col+width );
			if(bt != BlockNull && bt != BlockTree)return false;

			m_enemy[i].add_row(speed);
			for(int k=0;k<m_enemy_living;k++){
				if(k == i)continue;
				if(m_enemy[k].check_hit(m_enemy[i])){
					m_enemy[i].add_row(-speed);
					return false;
				}
			}
			for(int k=0;k<=!m_single_player;k++){
				if(m_enemy[i].check_hit(m_player[k])){
					m_enemy[i].add_row(-speed);
					return false;
				}
			}

			//no need to enemy bore
			
			for(int j=0;j<=!m_single_player;j++){
				if(m_player_bore_living[j]){
					if(m_enemy[i].check_hit(m_player_bore[j])){
						m_enemy[i].add_row(-speed);
						return false;
					}
				}
			}

			return true;

		case DirLeft:
			if(begin_col - speed  < 0 )return false;
			bt = m_map.pixel_type(begin_row , begin_col-speed);
			if(bt != BlockNull && bt != BlockTree)return false;
			bt = m_map.pixel_type(begin_row + 16, begin_col-speed);
			if(bt != BlockNull && bt != BlockTree)return false;
			bt = m_map.pixel_type(begin_row + height ,begin_col - speed); 
			if(bt != BlockNull && bt != BlockTree)return false;

			m_enemy[i].add_col(-speed);
			for(int k=0;k<m_enemy_living;k++){
				if(k == i)continue;
				if(m_enemy[k].check_hit(m_enemy[i])){
					m_enemy[i].add_col(speed);
					return false;
				}
			}

			for(int k=0;k<=!m_single_player;k++){
				if(m_enemy[i].check_hit(m_player[k])){
					m_enemy[i].add_col(speed);
					return false;
				}
			}

			for(int j=0;j<m_enemy_bore_living;j++){
				if(m_enemy[i].check_hit(m_enemy_bore[j])){
					m_enemy[i].add_col(speed);
					return false;
				}
			}

			for(int j=0;j<=!m_single_player;j++){
				if(m_player_bore_living[j]){
					if(m_enemy[i].check_hit(m_player_bore[j])){
						m_enemy[i].add_col(speed);
						return false;
					}
				}
			}
			return true;

		default:
			return false;
	}
}




void Game::create_bonus(){
	m_bonus_living = true;
	//m_bonus.create(BonusHelmet,game_random(100,350),game_random(100,350));
	int type = game_random(0, 5);
	int row = game_random(100, 350);
	int col = game_random(100, 350);
	m_bonus.create((BonusType)type,row,col);
	//m_bonus.create(BonusBomb,game_random(100,350),game_random(100,350));
	//onCache_CREATE_BONUS(type, row, col,pClient1,pClient2);

	//send_order here CREATE_BONUS->BonusType->row->col
}

void Game::draw_screen_on_run(){
	//画整个屏幕
	
//	debug("in draw screen on run\n");
	draw_periphery();
//	debug("in blit floor =================================\n");
	m_map.blit_floor(m_buffer,row_off,col_off);
//	debug("out blit floor =================================\n");
	draw_sprite();
	m_map.blit_cover(m_buffer,row_off,col_off);
	update_screen();
//	debug("out draw screen on run===============================\n");
}


void Game::draw_enemy_left(){
	char ll[5];
	sprintf(ll,"%d",m_enemy_left + m_enemy_living+m_enemy_bore_living);
	m_pen.put_str(m_buffer,ll,388,563);
}
void Game::draw_player_life_left(){
	char ll[5];
	sprintf(ll,"%d",m_player_life_left[0]);
	m_pen.put_str(m_buffer,ll,168,545);

	if(!m_single_player){
		sprintf(ll,"%d",m_player_life_left[1]);
		m_pen.put_str(m_buffer,ll,289,545);
	}
}
void Game::draw_game_level(){
	char ll[5];
	sprintf(ll,"%d",m_level);
	m_pen.put_str(m_buffer,ll,77,568);
}
void Game::draw_player_level(){
	char ll[5];
	sprintf(ll,"%d",m_player_level[0]);
	m_pen.put_str(m_buffer,ll,205,545);
	if(!m_single_player){
		sprintf(ll,"%d",m_player_level[1]);
		m_pen.put_str(m_buffer,ll,323,545);
	}
}
void Game::draw_periphery(){
	//画周边
	
	//清扫四周的炸弹痕迹
	
//	debug("in draw_periphery====================================\n");
	m_buffer.draw(m_img_world,0,0,640,480,0,0);
	draw_enemy_left();
	draw_player_life_left();
	draw_player_level();
	draw_game_level();

	debug1("m_msg_click = %d\n",m_msg_click);
	if(m_msg_click >0)
		m_pen.put_str(m_buffer,m_msg.c_str(),450,64);

//	debug("out draw_periphery=======================================\n");

}


void Game::draw_sprite(){
//	debug("in draw sprite\n");
	for(int i=0;i<=!m_single_player;i++){ //player以及player的炸弹
		if(m_player_bore_living[i])
			m_player_bore[i].blit(m_buffer,row_off,col_off);
		for(int j=0;j<m_player_bullet_living[i];j++)
			m_player_bullet[i][j].blit(m_buffer,row_off,col_off,RGB(0,0,0));
		if(m_player_living[i])
			m_player[i].blit(m_buffer,row_off,col_off);
	}

	for(int i=0;i<m_enemy_bullet_living;i++) //敌人的炸弹
		m_enemy_bullet[i].blit(m_buffer,row_off,col_off,RGB(0,0,0));
	for(int i=0;i<m_enemy_bore_living;i++) //敌人的出生
		m_enemy_bore[i].blit(m_buffer,row_off,col_off);
	for(int i=0;i<m_enemy_living;i++)//敌人
		m_enemy[i].blit(m_buffer,row_off,col_off);
	for(int i=0;i<m_small_explode_living;i++) //打砖 
		m_small_explode[i].blit(m_buffer,row_off,col_off,RGB(0,0,0));
	for(int i=0;i<m_big_explode_living;i++) //敌人死 自己死
		m_big_explode[i].blit(m_buffer,row_off,col_off,RGB(0,0,0));
	if(m_hawk_explode_living)
		m_hawk_explode.blit(m_buffer,row_off,col_off,RGB(0,0,0));
	if(m_bonus_living)
		m_bonus.blit(m_buffer,row_off,col_off);
//	debug("out draw sprite\n");
}

void Game::create_small_explode(int row,int col){
	m_small_explode[m_small_explode_living].create(ExplodeTile,row,col);
	m_small_explode_living ++;
}
void Game::create_big_explode(int row,int col){
	m_big_explode[m_big_explode_living].create(ExplodeTank,row,col);
	m_big_explode_living ++;
}
void Game::create_hawk_explode(){
	m_hawk_explode.create(ExplodeHawk,12*32+16,6*32+16);
	m_hawk_explode_living = true;
}
void Game::create_enemy_bore(int i){
	m_enemy_bore[m_enemy_bore_living].create(16,16+32*6*(i));
	m_enemy_bore_living ++ ;
	m_enemy_left --;
	m_last_enemy_bore_click = 0;
}
void Game::create_player_bore(int i){
	debug("in create_player_bore\n");
	m_player_bore[i].create(16+12*32,16+(4+i*4)*32);
	m_player_bore_living[i] = true;
}

void Game::create_enemy_bullet(int i){
	Direction md=m_enemy[i].dir();
	m_enemy_bullet[m_enemy_bullet_living].create(
			(BulletLevel)m_enemy[i].level(),
			md,
			m_enemy[i].row() + m_enemy[i].hot_row()*((md+1)%2 - (int)(md == 0)*2),
			m_enemy[i].col() + m_enemy[i].hot_col()*(    md%2 - (int)(md == 3)*2)
			);
	m_enemy_bullet_living ++;
	debug("create enemy bullet .............\n");
}
void Game::create_player_bullet(int i){
	m_player_bullet[i][m_player_bullet_living[i]].create(
			(BulletLevel)m_player[i].level(),
			m_player[i].dir(),
			m_player[i].row() + m_player[i].hot_row()*((m_player[i].dir()+1)%2 - (int)(m_player[i].dir() == 0)*2),
			m_player[i].col() + m_player[i].hot_col()*(    m_player[i].dir()%2 - (int)(m_player[i].dir() == 3)*2)
			);
	m_player_bullet_living[i] ++;
}
void Game::init_map(){
	char path[40];
	sprintf(path,"./map/level%d.map",m_level);
	
	log("ERROR LOCATED\r\n");
	m_map.load(path);
	log("ERROR PASSED\r\n");
}

//void Game::lock_game(){
//	pthread_mutex_lock(&m_mutex);
//}
//void Game::unlock_game(){
//	pthread_mutex_unlock(&m_mutex);
//}
//bool Game::try_lock_game(){
//	return !pthread_mutex_trylock(&m_mutex) ;
//}

void Game::set_single_player(bool fl){
	m_single_player = fl;
}

void Game::remove_player(int i){
	m_player_living[i] = false;
	m_player_life_left[i] --;
	m_player_level[i] = 0;
}



void Game::draw_screen_on_start(){
//	debug("in draw_screen_on_start\n");
	update_screen();
//	debug("out draw_screen_on_start\n");

}


/*
	click:
		->m_select_click
		->setState[if m_select_click > click(15*1000)] = GameSplash
*/
void Game::update_auto_data_on_select(){
	//m_buffer
	m_select_click ++;
	if(m_select_click > click(15*1000))
		set_state(GameSplash);

//	debug("in update_data_on_selset\n");
//	debug("out update_data_on_selset\n");
}

void Game::over(){	
	if (pClient1 != NULL) {
		log("[INFO::DISCONNECT_GAME_END]%s in over \r\n", pClient1->m_IP);
		DisconnectClient(pClient1);
	}
	if (pClient2 != NULL) {
		log("[INFO::DISCONNECT_GAME_END]%s in over \r\n", pClient2->m_IP);
		DisconnectClient(pClient2);
	}
}

void Game::remove_player_bore(int i){
	m_player_bore_living[i] = false;
}

/*
	随机产生的部分：enemy的等级
*/
void Game::create_enemy(int row,int col){

	//create中无m_enemy_left --  
	//因为这在create_enemy_bore中
	//其实这里的m_enemy_living应该叫m_enemy_active 更合适一些
	int a,b;
	a=(int)(game_random(0,1)==0);
	b=(int)(game_random(0,3)==0);
	bool flash = (game_random(0, 9) == 0);
//	m_enemy[m_enemy_living].create((EnemyType)(a+b),true,row,col);
	m_enemy[m_enemy_living].create((EnemyType)(a+b),flash,row,col);
	m_enemy[m_enemy_living].add_level(m_enemy_level);
	m_enemy_living ++ ;

	//onCache_CREATE_ENEMY(row, col, a + b, flash,pClient1,pClient2);

}


void Game::create_player(int i){
	m_player[i].create((PlayerType)i,16+12*32,16+(4+i*4)*32);
	m_player[i].add_level(m_player_level[i]);
	m_player_living[i] = true;
}
/*
	诞生与撤销机制
	诞生：调用create_enemy_bore(cou)
	撤销：执行
				m_enemy_bore_living --;
				m_enemy_left ++;
				m_last_enemy_bore_click = tmp;
		
*/
void Game::try_create_enemy_bore(){

	debug("cd create enemy bore if need and could\n");
	static int cou=0;
	if(m_enemy_living < 6 	   && 
	   m_enemy_left > 0   	   && 
	   m_enemy_bore_living < 3 && 
	   (m_last_enemy_bore_click >click(3*1000) || m_enemy_left > 17)){
		//need...

		//以下检测能否现在生成
		//主要是在生成地看看是否有车
		//
		//先生成,再检测,有问题就马上撤掉

		int tmp = m_last_enemy_bore_click;
		create_enemy_bore(cou);
		//onCache_CREATE_ENEMY_BORE(cou, pClient1, pClient2);

		for(int i =0;i<m_enemy_living;i++){
			if(m_enemy[i].check_hit(m_enemy_bore[m_enemy_bore_living-1])){
				m_enemy_bore_living --;
				m_enemy_left ++;
				m_last_enemy_bore_click = tmp;
				//onCache_BACK_OFF_CREATE_ENEMY_BORE(tmp, pClient1, pClient2);
				return;
			}
		}
		for(int i=0;i<=!m_single_player;i++){
			if(m_player_living[i] && m_player[i].check_hit(m_enemy_bore[m_enemy_bore_living-1])){
				m_enemy_bore_living --;
				m_enemy_left ++;
				m_last_enemy_bore_click = tmp;
				//`onCache_BACK_OFF_CREATE_ENEMY_BORE(tmp, pClient1, pClient2);
				return;
			}
		}
		cou ++;
		cou%=3;
	}
	debug(" out  create enemy bore if need and could\n");
}

/*
	新生的条件：
	此时坦克已死+坦克仍有剩余生命+坦克尚未处于新生状态
*/
void Game::try_create_player_bore(){
	debug("cd create player bore if need and could\n");
	for(int i=0;i<=(int)!m_single_player;i++){
		if( !m_player_living[i] 		&& 
			m_player_life_left[i]>0 	&& 
			!m_player_bore_living[i]){

				create_player_bore(i);
				for(int j =0;j<m_enemy_living;j++){
					if(m_enemy[j].check_hit(m_player_bore[i])){
						remove_player_bore(i);
						return;
					}
				}
				if(!m_single_player  			&& 
				   m_player_living[(int)!i]		&&
				   m_player_bore[i].check_hit(m_player[(int)!i])){
						remove_player_bore(i);
						return;
				}
		}
	debug("out create player bore if need and could\n");
	}
}



void Game::set_enemy_freezing(){
	m_enemy_freezing = true;
	m_enemy_freezing_click = 0;
}

void Game::kill_enemy(){
	for(int i=0;i!=m_enemy_living;i++)
		create_small_explode(m_enemy[i].row(),m_enemy[i].col());
	m_enemy_living = 0;
}

void Game::deal_with_enemy_bullet(){
	enemy_bullet_hit_player(); //可能发送命令KILL_PLAYER_J REMOVE_ENEMY_BULLET

	enemy_bullet_hit_tile();	//暂定客户定保留运行该段代码							
	//onCache_CALL_ENEMY_BULLET_HIT_TILE(pClient1,pClient2);
	enemy_bullet_hit_fence();  //暂定客户定保留运行该段代码	
//	onCache_CALL_ENEMY_BULLET_HIT_FENCE(pClient1, pClient2);
	enemy_bullet_update();		//暂定客户定保留运行该段代码
//	onCache_CALL_ENEMY_BULLET_UPDATE(pClient1, pClient2);
}

void Game::deal_with_player_bullet(){
	player_bullet_hit_enemy(); //可能发送的命令：CREATE_BONUS KILL_ENEMY_K REMOVE_PLAYER_BULLET_I_J
	
	player_bullet_hit_player(); //可能发送的命令：PLAYER_BULLET_HIT_PLAYER

	player_bullet_hit_player_bullet();//可能发送的命令：PLAYER_BULLET_HIT_PLAYER_BULLET
	player_bullet_hit_enemy_bullet(); //可能发送的命令:PLAYER_BULLET_HIT_ENEMY_BULLET

	player_bullet_hit_tile();	//影响地图，暂定由客户端自行处理
	//onCache_CALL_PLAYER_BULLET_HIT_TILE(pClient1,pClient2);


	player_bullet_hit_fence();	//PLAYER_BULLET_HIT_FENCE

	player_bullet_update();	//由客户端自行处理
}

void Game::enemy_bullet_hit_player(){
	int i;

	for(int j=0;j<=(int)!m_single_player;j++){
		if(m_player_living[j]){

			i = 0;
			while(i != m_enemy_bullet_living){
				if(m_enemy_bullet[i].check_hit(m_player[j])){
					//不幸击中
					if(m_player[j].hitted()){
						//不幸死亡(没带套子)
						create_big_explode(m_player[j].row(),
								           m_player[j].col());
						remove_player(j);
						m_audio.play_sound(SoundTankHitted);

						//onCache_KILL_PLAYER_J(j,pClient1,pClient2);
					}
					//删去此bullet
					remove_enemy_bullet(i);
					//onCache_REMOVE_ENEMY_BULLET(i, pClient1,pClient2);
				}else{
					//检查下一个
					i ++;
				}
			}
		}
	}
}

/*
	检查越界
*/
void Game::enemy_bullet_hit_fence(){
	int ind = 0;

	while(ind != m_enemy_bullet_living){
		if(m_enemy_bullet[ind].row() >= 13*32 ||
		   m_enemy_bullet[ind].row() <= 0     ||
		   m_enemy_bullet[ind].col() >= 13*32 ||
		   m_enemy_bullet[ind].col() <= 0     ){

			//如果越界了就摆闸
			create_small_explode(m_enemy_bullet[ind].row(),m_enemy_bullet[ind].col());
//			m_audio.play_sound(SoundBlockHitted);


			remove_enemy_bullet(ind);
		}else{
			ind ++;
		}
	}
}

void Game::enemy_bullet_update(){
	for(int ind=0;ind<m_enemy_bullet_living;ind++){
		m_enemy_bullet[ind].move();
	}
}

void Game::player_bullet_hit_enemy(){
	//第i个玩家的第j颗子弹击中第k个敌人?
	debug("in player_bullet_hit_enemy\n");
	int i,j,k;
	
	//这段程序的逻辑要仔细检查
	for(i=0 ; i<=(int)!m_single_player ; i++){
		if(m_player_living[i]){

			j = 0;
			lab_while1:
			while(j != m_player_bullet_living[i]){

				k = 0;
				while(k != m_enemy_living){
					if(m_player_bullet[i][j].check_hit(m_enemy[k])){
						//撞上了

			//			debug3("第%d个玩家的第%d颗子弹击中第%d个敌人...\n",i,j,k);
						//敌人会闪,必不死
						if(m_enemy[k].flash()){
						//	debug1("第%d个敌人会闪\n",k);
							create_bonus();
						}
						if(m_enemy[k].hitted()){//死
							m_count[i][(int)(m_enemy[k].type())+1] ++;
							create_small_explode(m_enemy[k].row(),m_enemy[k].col());
							m_audio.play_sound(SoundTankHitted);
							remove_enemy(k);

							//onCache_KILL_ENEMY_K(i,k,pClient1,pClient2);
						}
						//只要撞上 必然移出bullet

						debug2("移出第%d玩家的第%d个子弹\n",i,j);
						remove_player_bullet(i,j);
						//onCache_REMOVE_PLAYER_BULLET_I_J(i, j, pClient1, pClient2);
						debug2("第%d玩家剩下%d个子弹\n",i,m_player_bullet_living[1]);
						//这第j个子弹
						//都被移出了,后面的敌人不用检查了
						//跳出时候的j也不需要自增,因为此时的j已经是下一个了
						//break;
						//为什么这里用break不行:这里break后会执行j++;
						goto lab_while1;


					}else{//没撞上 下一个敌人
						k ++;
					}
					//第k个敌人检查完毕
				}
				j ++;
				//对于第j个子弹,检查完毕,j自增
			}
		}
	}
	debug("out player_bullet_hit_enemy\n");
}


void Game::player_bullet_hit_player(){
	debug("cd player bullet hit player\n");
	if(m_single_player     ||
	   !m_player_living[0] ||
	   !m_player_living[1]
	   )return;

	int i,j;

	i = 0;
	while( i < 2){


		if(m_player[!i].shield()){
			i ++;
			continue;
		};


		//第i个玩家的第j颗子弹
		j = 0;

		while(j != m_player_bullet_living[i]){
			if( m_player_bullet[i][j].check_hit(m_player[!i])){
				//不幸击中
				m_player[!i].set_state(PlayerWink);
				remove_player_bullet(i,j);
				//onCache_PLAYER_BULLET_HIT_PLAYER(i,j,pClient1,pClient2);
			}else{
				j ++;
			}
		}
		i++;
	}
	debug("out player bullet hit player\n");
}

void Game::player_bullet_hit_player_bullet(){

	if(m_single_player)return;
	int i,j;
	//第0个player的第i子弹和第一个人的第j子弹:
	
	i = 0;
	while( i!= m_player_bullet_living[0]){
		j = 0;
		while(j != m_player_bullet_living[1]){

			if(m_player_bullet[0][i].check_hit(m_player_bullet[1][j])){

				create_small_explode(m_player_bullet[0][i].row(),m_player_bullet[0][i].col());
				remove_player_bullet(0,i);
				remove_player_bullet(1,j);

				//onCache_PLAYER_BULLET_HIT_PLAYER_BULLET(i, j,pClient1,pClient2);
				break;
			}else{
				j ++;
			}
		}
		i ++;
	}
}


void Game::player_bullet_hit_tile(){

	int ind0,ind1;
	Tile* pt0;
	Tile* pt1;
	Direction dir;
	int row,col;
	bool need_create_small_explode ;
	bool need_create_hawk_explode ;

	int cou ;

	for(int i=0;i<=(int)!m_single_player;i++){
		cou = 0;
		while(cou != m_player_bullet_living[i]){
	
			row = m_player_bullet[i][cou].row();
			col = m_player_bullet[i][cou].col();
			dir = m_player_bullet[i][cou].dir();
	
			get_info_of_hit(&pt0,&ind0,&pt1,&ind1,row,col,dir);

			need_create_small_explode = false;
			need_create_hawk_explode = false;
	
			if(pt0->type(ind0)==BlockBrick){
				need_create_small_explode = true;

				destroy_tile_block(pt0,ind0);

			}else if(pt0->type(ind0)==BlockConcrete){
				need_create_small_explode = true;

				if((int)m_player_bullet[i][cou].level() > (int)BulletLevel2){
					destroy_tile_block(pt0,ind0);
				}
			}else if(pt0->type(ind0)==BlockHawk){
				need_create_hawk_explode = true;
				pt0->set_type(BlockStone);
				pt0->set_type(0,BlockStone);
				pt0->set_type(1,BlockStone);
				pt0->set_type(2,BlockStone);
				pt0->set_type(3,BlockStone);
			}else if(pt0->type(ind0)==BlockStone){
				need_create_small_explode = true;
			}
	
			if(pt1->type(ind1)==BlockBrick){
				need_create_small_explode = true;
				destroy_tile_block(pt1,ind1);
			}else if(pt1->type(ind1)==BlockConcrete){
				need_create_small_explode = true;
				if((int)m_player_bullet[i][cou].level() > (int)BulletLevel2){
					destroy_tile_block(pt1,ind1);
				}
			}else if(pt1->type(ind1)==BlockHawk){
				need_create_hawk_explode = true;
				pt1->set_type(BlockStone);
				pt1->set_type(0,BlockStone);
				pt1->set_type(1,BlockStone);
				pt1->set_type(2,BlockStone);
				pt1->set_type(3,BlockStone);
			}else if(pt1->type(ind1)==BlockStone){
				need_create_small_explode = true;
			}
	
			if(need_create_hawk_explode){
				//需要制造explode,由于为两次,如果在里面实现,可能一个炸弹被执行两次生产explode
				create_hawk_explode();
				m_audio.play_sound(SoundHawkHitted);
				remove_player_bullet(i,cou);
			}else if (need_create_small_explode){
				create_small_explode(row,col);
//				m_audio.play_sound(SoundBlockHitted);
				remove_player_bullet(i,cou);
			}else{
				cou ++;
			}
		}
	}
}

/*
	根据各敌军子弹当前的位置，决定地图的改变和是否进行相应的爆炸；
	被动信息交给客户端来实现
*/
void Game::enemy_bullet_hit_tile(){

	int ind0,ind1;
	Tile* pt0;
	Tile* pt1;
	Direction dir;
	int row,col;
	bool need_create_small_explode ;
	bool need_create_hawk_explode ;

	int cou ;
	debug("come in enemy_bullet_hit_tile\n");

	cou = 0;

	while(cou != m_enemy_bullet_living){

		/*
			拿到该敌方子弹的位置信息
		*/
		row = m_enemy_bullet[cou].row();
		col = m_enemy_bullet[cou].col();
		dir = m_enemy_bullet[cou].dir();

		/*
			拿到该子弹影响的两个Block信息(Tile:BlockNum)
		*/
		get_info_of_hit(&pt0,&ind0,&pt1,&ind1,row,col,dir);

//		if(pt0 == pt1 )debug("OK\n");

//		debug1("pt0 = %u\n",pt0);
//		debug1("pt1 = %u\n",pt1);

//		debug1("ind0 = %d\n",ind0);
//		debug1("ind1 = %d\n",ind1);
		need_create_small_explode = false;
		need_create_hawk_explode = false;

		if(pt0->type(ind0)==BlockBrick){
			//如果影响了一块砖
			
			need_create_small_explode = true;
			destroy_tile_block(pt0,ind0);
		}else if(pt0->type(ind0)==BlockConcrete){

			//如果影响的是一块钢板

			//不管该子弹当前的级别是否能够打穿钢板，爆炸是一定的
			need_create_small_explode = true;
			//BulletLevel0-3对应四种子弹等级

			//如果敌军子弹的等级不是最低级，就可以破坏该块钢板
			if((int)m_enemy_bullet[cou].level() > (int)BulletLevel0){
				destroy_tile_block(pt0,ind0);
			}


		}else if(pt0->type(ind0)==BlockHawk){

			//如果影响的是BlockHawk的话

			need_create_hawk_explode = true;
			pt0->set_type(BlockStone);
			pt0->set_type(0,BlockStone);
			pt0->set_type(1,BlockStone);
			pt0->set_type(2,BlockStone);
			pt0->set_type(3,BlockStone);

		}else if(pt0->type(ind0)==BlockStone){

			//如果影响的是BlockStone的话——死掉的老王
			need_create_small_explode = true;
		}

		if(pt1->type(ind1)==BlockBrick){
			need_create_small_explode = true;
			destroy_tile_block(pt1,ind1);
		}else if(pt1->type(ind1)==BlockConcrete){
			need_create_small_explode = true;
			if((int)m_enemy_bullet[cou].level() > (int)BulletLevel0){
				destroy_tile_block(pt1,ind1);
			}
		}else if(pt1->type(ind1)==BlockHawk){
			need_create_hawk_explode = true;
			pt1->set_type(BlockStone);
			pt1->set_type(0,BlockStone);
			pt1->set_type(1,BlockStone);
			pt1->set_type(2,BlockStone);
			pt1->set_type(3,BlockStone);
		}else if(pt1->type(ind1)==BlockStone){
			need_create_small_explode = true;
		}

		if(need_create_hawk_explode){
			//需要制造explode,由于为两次,如果在里面实现,可能一个炸弹被执行两次生产explode
			create_hawk_explode();
			m_audio.play_sound(SoundHawkHitted);
			remove_enemy_bullet(cou);
		}else if (need_create_small_explode){
			create_small_explode(row,col);
//			m_audio.play_sound(SoundBlockHitted);
			remove_enemy_bullet(cou);
		}else{
			cou ++;
		}
//		debug("no error in one\n");
	}
	debug("out enemy_bullet_hit_tile\n");
}

void Game::player_bullet_hit_fence(){
	debug("cd player bullet hit fence\n");
	int j;
	for (int i=0;i<=(int)!m_single_player;i++){
		j = 0;
		while(j != m_player_bullet_living[i]){
			if(m_player_bullet[i][j].row() >= 13*32 ||
			   m_player_bullet[i][j].row() <= 0     ||
			   m_player_bullet[i][j].col() >= 13*32 ||
			   m_player_bullet[i][j].col() <= 0     ){
				create_small_explode(m_player_bullet[i][j].row(),m_player_bullet[i][j].col());
//				m_audio.play_sound(SoundBlockHitted);
				remove_player_bullet(i,j);

				//onCache_PLAYER_BULLET_HIT_FENCE(i, j,pClient1,pClient2);
			}else{
				j ++;
			}
		}
	}
	debug("out player bullet hit fence\n");
}


void Game::player_bullet_update(){

	debug("in player bullet update \n");
	for(int i=0;i<=(int)!m_single_player;i++)
		for(int j=0;j<m_player_bullet_living[i];j++)
			m_player_bullet[i][j].move();
	debug("out player bullet update \n");

	//onCache_ALL_PLAYER_BULLET_MOVE(pClient1,pClient2);
}


/*
	if no bonus now then return 
	else
		->update Bonus::m_last_click[if time out then set it false]
		->判断是否有敌人吃到了bonus，如果是，执行对应动作并return
		->判断是否有player吃到了bonus，如果是，执行对应的动作并return
*/
void Game::deal_with_bonus(){
	if(!m_bonus_living)return;

	m_bonus.update();
	
	if(m_bonus.time_out()){
		m_bonus_living = false;
	}
	//敌人吃bonus
	for(int i=0;i<m_enemy_living;i++){
		if( m_bonus.check_hit(m_enemy[i])){

			m_bonus_living = false;
			m_audio.play_sound(SoundEnemyGetBonus);
			switch (m_bonus.type()){
				case BonusLife:
					//all add life
					for(int j=0;i<m_enemy_living;j++)
						m_enemy[j].add_life(1);
					show_message("All enemy add life ...",9000);
					break;

				case BonusClock:
					m_player_freezing = true;
					m_player_freezing_click = 0;
					show_message("All player freezing....",9000);
					break;

				case BonusShovel:
					show_message("Lost shovel ,Hawk in danger !!!",9000);
					m_map.set_hawk_bare();
					break;

				case BonusBomb:
					show_message("Kill all players ...",4000);
					for(int j=0;j<=(int)!m_single_player;j++){
						if(m_player_living[j]){
							remove_player(i);
							create_big_explode(m_player[j].row(),m_player[j].col());
						}
					}
					break;
				case BonusStar:
					show_message("All enemy level up...",7000);
					m_enemy_level ++;
					for(int k=0;k<m_enemy_living;k++)
						m_enemy[k].add_level(1);
					break;
				case BonusHelmet:
					show_message("One enemy unbeatable...",5000);
					m_enemy[i].set_shield(true);
					break;
				default:
					break;
			}
			return;

		}
	}

	//玩家吃bonus
	for(int i=0;i<=!m_single_player;i++){
		char str[100];
		if(m_player_living[i] && m_bonus.check_hit(m_player[i])){
			m_audio.play_sound(SoundPlayerGetBonus);
			m_count[i][0] ++; 
			m_bonus_living = false;
			switch (m_bonus.type()){
				case BonusLife:
					sprintf(str,"Player%d add one life...",i);
					show_message(str,6000);
					m_player_life_left[i]++;
					break;
				case BonusClock:
					show_message("All enemy freezing ...",8000);
					set_enemy_freezing();
					break;
				case BonusShovel:
					show_message("Hawk safed",5000);
					m_map.set_hawk_protected();
					break;
				case BonusBomb:
					show_message("Kill all enemy ...",5000);
					kill_enemy();
					break;
				case BonusStar:
					sprintf(str,"Player%d level up ...",i);
					show_message(str,6000);
					m_player[i].add_level(1);
					break;
				case BonusHelmet:
					sprintf(str,"Player%d unbeatable ...",i);
					show_message(str,6000);
					m_player[i].set_shield(true);
					break;
				default:
					break;
			}
			return;
		}
	}
}

/*
	1.判断游戏是否胜利 if so ,return 
	2.对现存的enemy:
		->尝试移动
		->更新m_shield_click、m_last_attack_click
		->尝试射击：
			call enemy_try_attack()
	3.更新 m_enemy_freezing_click [if m_enemy_freezing]
*/

void Game::deal_with_enemy(){
	if(m_enemy_living + m_enemy_left + m_enemy_bore_living == 0){
		if(m_win_delay_click == 0)
			m_win_delay_click = 1;
		if(m_win_delay_click == 1)
			show_message("Well done ...",3500);
		return;
	}
	//onCache_CALL_DEAL_WITH_ENEMY_UPDATE_WIN_DELAY_CLICK(pClient1,pClient2);


	for(int i=0;i<m_enemy_living;i++){
		if(!m_enemy_freezing){
			if(!enemy_try_go_ahead(i)){
				debug("enemy can not go ahead ,change dir ...\n");
				int dir = game_random(0, 3);
				m_enemy[i].set_dir((Direction)dir);//不能走立即换向，但可能换到原来的向
				//onCache_CALL_ENEMY_SET_DIR(pClient1, pClient2, i, dir);
			}
		//	else {
		//		//说明成功进行了移动，那么根据当前enemy的方向发送移动的数据包
		//		//进行到这里，服务器数据的修改在enemy_try_go_ahead里面已经完成了修改，这里只是为了将修改传给服务器端
		//		switch (m_enemy[i].dir()) {
		//		case DirUp:
		//			onCache_CALL_DEAL_WITH_ENEMY_UP(pClient1,pClient2,i);
		//			break;
		//		case DirDown:
		//			onCache_CALL_DEAL_WITH_ENEMY_DOWN(pClient1, pClient2, i);
		//			break;
		//		case DirLeft:
		//			onCache_CALL_DEAL_WITH_ENEMY_LEFT(pClient1, pClient2, i);
		//			break;
		//		case DirRight:
		//			onCache_CALL_DEAL_WITH_ENEMY_RIGHT(pClient1, pClient2, i);
		//			break;
		//		default:
		//			break;

		//	}
		}
		//多次强调,update不包含对位置信息的修改
		/*
				update click conditionly:
					->m_shield_click[if m_shield]
					->m_last_attack_click
		*/
		m_enemy[i].update();

		/*
			
		*/
		enemy_try_attack(i);

		//onCache_CALL_ENEMY_UPDATE_ATTACK(pClient1, pClient2, i);
		
	}

	//enemy is freezing..
    if(	m_enemy_freezing){
		m_enemy_freezing_click ++;
		if(m_enemy_freezing_click >= click(1000*5))
			m_enemy_freezing = false;
	}

}


/*
	deal with:
		->所有小型爆炸[click]
		->所有大型爆炸[click]
		->鹰爆炸[if living ][click]
*/
void Game::deal_with_explode(){

	//small explode
	int i;
	i=0;
	while(i != m_small_explode_living){
		m_small_explode[i].update();
		if(m_small_explode[i].time_out()){
			remove_small_explode(i);
		}else{
			i ++;
		}
	}

	i= 0;
	while(i != m_big_explode_living){
		m_big_explode[i].update();
		if(m_big_explode[i].time_out()){
			remove_big_explode(i);
		}else{
			i ++;
		}
	}


	/*
		执行hawk爆炸
		爆炸结束之后，游戏失败结束——置m_over_delay_click = 1
	*/
	if(m_hawk_explode_living){
		m_hawk_explode.update();
		if(m_hawk_explode.time_out()){
			m_hawk_explode_living = false;
			m_over_delay_click = 1;
		}
	}
}

/*
	click:
		->m_over_delay_click
		->m_shield_click
		->m_wink_click(game state click)
		->m_player_freezing_click

	relationship：
		1.第一种click执行则后面三种不会执行
		2.后三种click可以都执行

	function：
		检查当前player的状态和游戏状态(PlayerWink),递进对应click
			-> if gameover then set m_over_delay_click
					return
			-> if m_shield then xxx

			-> if PlayerWink then xxx

			-> if m_player_freezing_click then xxx
*/

void Game::deal_with_player(){

	/*
		judge life if 0 to decide if fail ——综合双人模式考量
			-> by !
		设置延时over_delay_click
	*/
	if((m_player_life_left[0] + !m_single_player*m_player_life_left[1]) == 0){
		if(m_over_delay_click == 0)
			m_over_delay_click = 1;
		if(m_over_delay_click == 1)
			show_message("Fail ...",3000);
		return;
	}


	//如果某玩家死亡的话，对于该玩家而言，游戏结束

	/*
		update——综合双人模式考量
			-> by !
	*/
	for(int i=0;i<=!m_single_player;i++){

		/*
			FUNCTION:
				1.检查无敌状态m_shield——非游戏状态的一种，递进click
				-> 超过click(8000)帧数则置为false；

				2.对于游戏状态为PlayerWink（闪烁）的时候，递进click
				-> 超过click(8000)帧数则置状态为PlayerNormal

			NOTICE: DIDNOT MOVE!
		*/

		m_player[i].update();
	}

	/*
		处理冷冻状态m_player_freezing
	*/
	if(m_player_freezing){
		m_player_freezing_click ++;
		if(m_player_freezing_click > click(10*1000)){
			m_player_freezing = false;
			m_player_freezing_click = 0;
		}
	}
}



	

/*
	这里有一个很明显的渐进变化过程
	这个过程完全是依赖于其他数据的正确性，也就是说：在服务器端和客户端数据保持一致的情况下
*/
void Game::deal_with_player_bore(){
	try_create_player_bore();

	for (int i=0;i<=!m_single_player;i++){
		if(m_player_bore_living[i]){
			m_player_bore[i].update();
			if(m_player_bore[i].time_out()){
				remove_player_bore(i);
				create_player(i);
				m_player[i].set_shield(true);
			}
		}
	}

	//在数据一致的前提下，上述代码的执行结果在服务器端、两个客户端都应该是一致的

	//onCache_DEAL_WITH_PLAYER_BORE(pClient1,pClient2);
}



void Game::deal_with_enemy_bore(){
	debug("cd deal with enemy bore\n");
	int i;
	m_last_enemy_bore_click ++;
	//onCache_CALL_DEAL_WITH_ENEMY_BORE_UPDATE(pClient1, pClient2);

	try_create_enemy_bore();


	i = 0;
	while(i != m_enemy_bore_living){
		m_enemy_bore[i].update();//	Bore::m_last_click ++;
		//onCache_BORE_UPDATE_I(i,pClient1,pClient2);

		//制定数据包不要漏过任何逻辑的检查
		if(m_enemy_bore[i].time_out()){//判断可以单纯地放在服务器端
			//根据位置，随机选定等级生成敌军坦克
			create_enemy(m_enemy_bore[i].row(),m_enemy_bore[i].col());
			remove_enemy_bore(i);

		//	onCache_REMOVE_ENEMY_BORE_I(i,pClient1,pClient2);
		}else{
			i ++;
		}
	}
	debug("out deal with enemy bore\n");
}

void Game::show_message(const std::string& msg,int ms){
	m_msg_click = click(ms);
	m_msg= msg;
}

void Game::player_bullet_hit_enemy_bullet(){

	int j,k;
	for(int i=0;i<=(int)!m_single_player;i++){

		j=0;

		lab_while:
		while(j != m_player_bullet_living[i]){

			k=0;
			while(k != m_enemy_bullet_living){

				if(m_player_bullet[i][j].check_hit(m_enemy_bullet[k])){
					remove_player_bullet(i,j);
					remove_enemy_bullet(k);

					//onCache_PLAYER_BULLET_HIT_ENEMY_BULLET(i, j, k,pClient1,pClient2);
					goto lab_while;
				}
				k++;
			}
			j++;
		}
	}
}


void Game::deal_with_message(){

	if(m_msg_click >0)
		m_msg_click --;
}
/*
	click:
		->m_win_delay_click
		->m_over_delay_click

	logic:
		如果以上的click有被置成非0值，那么就会在这里被执行递增
		直到某个界限，分别设置游戏状态为GameWin或者GameOver
*/
void Game::deal_with_delay(){
	if(m_win_delay_click != 0){
		m_win_delay_click ++;
		if(m_win_delay_click == click(2*1000)){
			set_state(GameWin);
			return;
		}
	}
	if(m_over_delay_click != 0){
		//printf("in over delay click = %d\n",m_over_delay_click);
		m_over_delay_click ++;
		if(m_over_delay_click > click(2*1000)){
			set_state(GameOver);
			return;
		}
	}
}

/*
	通过Tile的序号和pindx对应的序号可以对应某个Tile的一个Block
*/
void Game::get_info_of_hit(Tile** pt0,int* pind0,
						   Tile** pt1,int* pind1,
						   int row,int col,
						   Direction dir){

	//机理:每个子弹必会影响两个block
	//这两个block可能为一个Tile,也可能在两个Tile
	

	switch (dir){
		//假定子弹的势力范围是一个16*16的正方形？？？

		case DirUp:
		case DirDown:
			*pt0 = &(m_map.pixel_tile(row,col-8));
			*pt1 = &(m_map.pixel_tile(row,col+8));

			*pind0 = tile_block_index(row,col-8);
			*pind1 = tile_block_index(row,col+8);
			break;
		case DirRight:
		case DirLeft:
			*pt0 = &(m_map.pixel_tile(row-8,col));
			*pt1 = &(m_map.pixel_tile(row+8,col));
			*pind0 = tile_block_index(row-8,col);
			*pind1 = tile_block_index(row+8,col);
			break;
	}

}

void Game::destroy_tile_block(Tile* pt,int ind){
	//进来的时候当前块就已经是砖,甲了 
	pt->set_type(ind,BlockNull);
	if(pt->type() != BlockMix){
		//原来为纯的
		pt->set_type(BlockMix);
	}else{
		//原来为混的
		if(pt->type(0)==BlockNull&&
		   pt->type(1)==BlockNull&&
		   pt->type(2)==BlockNull&&
		   pt->type(3)==BlockNull){
			//打成纯空了
			pt->set_type(BlockNull);
		}
	}
}

void Game::remove_player_bullet(int i,int j){
	for(int ind=j;ind!=m_player_bullet_living[i];ind++)
		m_player_bullet[i][ind] = m_player_bullet[i][ind+1];
	m_player_bullet_living[i]--;
}

void Game::remove_enemy_bullet(int i){
	for(int ind=i;ind!=m_enemy_bullet_living;ind++)
		m_enemy_bullet[ind] = m_enemy_bullet[ind+1];
	m_enemy_bullet_living--;
}
void Game::remove_enemy(int i){
	for(int ind=i;ind!=m_enemy_living;ind++)
		m_enemy[ind] = m_enemy[ind+1];
	m_enemy_living--;
}
void Game::remove_small_explode(int i){

	for(int ind=i;ind+1!=m_small_explode_living;ind++)	
		m_small_explode[ind] = m_small_explode[ind+1];
	m_small_explode_living --;
}
void Game::remove_big_explode(int i){

	for(int ind=i;ind+1!=m_big_explode_living;ind++)	
		m_big_explode[ind] = m_big_explode[ind+1];
	m_big_explode_living --;
}
void Game::remove_enemy_bore(int i){

	for(int j=i;j+1<m_enemy_bore_living;j++)
		m_enemy_bore[j]=m_enemy_bore[j+1];
	m_enemy_bore_living --;
}
