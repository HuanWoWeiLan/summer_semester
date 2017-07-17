/*****************************************��������*************************************************/
#include<Windows.h>
#include"LinkList.h"
#include<time.h>



#define CELL_PIXEL 23

#ifndef ZU_MA_H
#define ZU_MA_H

#define MAX_TRACK_STONE 133//����ϵı�ʯ���������

//��갴�����ʱ������
int theMouseX;
int theMouseY;

PLIST list_stones;
PLIST list_track;

double dir;//б��
//�������ɫ
typedef enum _TRACK_COLOR
{
	BLACK,
	RED,//�����յ�
}TRACK_COLOR;
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//���������
typedef struct  _TRACK
{
	int x;
	int y;
	//TRACK_COLOR color;

}TRACK, *LPTRACK;


//�����������
void create_track(
	int x,
	int y
	//TRACK_COLOR color
);

//��ʯ���� 
typedef enum _STONE_TYPE
{
	TRACK_STONE,//�����ԭ�еı�ʯ
	FROG_STONE,//�����³����ı�ʯ
}STONE_TYPE;

//��ʯ������״̬
typedef enum _STONE_STATE//ֻ��Ҫ������״̬
{
	STONE_HIT,
	STONE_MOVED,
}STONE_STATE;

//��ʯ����ɫ
typedef enum _STONE_COLOR
{
	YELLOW,
	ORANGE,
	BLUE,
	PINK��
}STONE_COLOR;


//��ʯ������
typedef struct STONE
{
	STONE_TYPE type;
	int x;
	int y;
	double dir;           // �ƶ�ʱ��x�����y
	int y_step;// y����ÿ��ʱ�������ƶ��ľ���
	STONE_STATE state;//FLY_STATE state,
	STONE_COLOR color;
	unsigned int power;
	int ratio_x;	         // �������Χ�ڻ��ж���Ч��
	int ratio_y;

}STONE, *LPSTONE;



//������ʯ����
LPSTONE create_stone(
	STONE_TYPE type,
	int x,
	int y,
	double dir,		           // �ƶ�ʱ��x�����y
	int y_step,	              // y����ÿ��ʱ�������ƶ��ľ���
	STONE_STATE state, //FLY_STATE state,
	STONE_COLOR color,
	unsigned int power,
	int ratio_x,		         // �������Χ�ڻ��ж���Ч��
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
PGAME_COORD GetTrackTail();//��ȡ��������һ���ڵ�

PGAME_COORD GetTrackAt(int n);

void TrackStoneMove();
void FrogStoneMove(LPSTONE stone);


#endif

