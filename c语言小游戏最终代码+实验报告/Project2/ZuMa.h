/*****************************************青蛙祖玛*************************************************/
#include<Windows.h>
#include"LinkList.h"
#include<time.h>



#define CELL_PIXEL 23

#ifndef ZU_MA_H
#define ZU_MA_H

#define MAX_TRACK_STONE 133//轨道上的宝石的最大数量

//鼠标按下左键时的坐标
int theMouseX;
int theMouseY;

PLIST list_stones;
PLIST list_track;

double dir;//斜率
//轨道的颜色
typedef enum _TRACK_COLOR
{
	BLACK,
	RED,//起点和终点
}TRACK_COLOR;
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//轨道的属性
typedef struct  _TRACK
{
	int x;
	int y;
	//TRACK_COLOR color;

}TRACK, *LPTRACK;


//创建轨道函数
void create_track(
	int x,
	int y
	//TRACK_COLOR color
);

//宝石类型 
typedef enum _STONE_TYPE
{
	TRACK_STONE,//轨道上原有的宝石
	FROG_STONE,//青蛙吐出来的宝石
}STONE_TYPE;

//宝石所处的状态
typedef enum _STONE_STATE//只需要有两个状态
{
	STONE_HIT,
	STONE_MOVED,
}STONE_STATE;

//宝石的颜色
typedef enum _STONE_COLOR
{
	YELLOW,
	ORANGE,
	BLUE,
	PINK，
}STONE_COLOR;


//宝石的属性
typedef struct STONE
{
	STONE_TYPE type;
	int x;
	int y;
	double dir;           // 移动时，x相对于y
	int y_step;// y方向每个时间周期移动的距离
	STONE_STATE state;//FLY_STATE state,
	STONE_COLOR color;
	unsigned int power;
	int ratio_x;	         // 在这个范围内击中都有效。
	int ratio_y;

}STONE, *LPSTONE;



//创建宝石函数
LPSTONE create_stone(
	STONE_TYPE type,
	int x,
	int y,
	double dir,		           // 移动时，x相对于y
	int y_step,	              // y方向每个时间周期移动的距离
	STONE_STATE state, //FLY_STATE state,
	STONE_COLOR color,
	unsigned int power,
	int ratio_x,		         // 在这个范围内击中都有效。
	int ratio_y
);
void stones_destory();
void destory_stone_at(unsigned int i);


unsigned int get_stone_num();
unsigned int get_track_num();
LPSTONE get_stone_at(unsigned int i);
void stone_move(LPSTONE stone);
unsigned int get_stone_x(LPSTONE stone);
unsigned int get_stone_y(LPSTONE stone);

void stone_be_hit(LPSTONE stone);
void be_hit(LPSTONE stone);


void destory_stone_by_state();
void destory();
PGAME_COORD GetTrackTail();//获取轨道的最后一个节点

PGAME_COORD GetTrackAt(int n);

void TrackStoneMove();
void FrogStoneMove(LPSTONE stone);


#endif

