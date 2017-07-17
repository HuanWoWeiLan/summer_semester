#include"ZuMa.h"
#include"LinkList.h"
#include<time.h>

DWORD dwSorce = 0;

extern LPSTONE stone;



//�������
void create_track(
	int x,   //��
	int y   //��
			//TRACK_COLOR color
)
{
	PGAME_COORD p;

	int i;

	list_track = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));

	// ���ͷ���ĳ�ʼλ�ã�
	p->x = 8;
	p->y = 1;
	ListPushFront(list_track, p);

	for (int i = 0;i < MAX_TRACK_STONE;i++)
	{
		PGAME_COORD pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
		int size = get_track_num();
		PGAME_COORD pTail = GetTrackTail();
		if (size < y)//��߽�
		{

			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y + 1;
		}
		else if (size >= y&&size < (x + y))//�±߽�
		{
			pNewTail->x = pTail->x + 1;
			pNewTail->y = pTail->y;
		}
		else if (size >= (x + y) && size <= (y + y + x))//�ұ߽�
		{

			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y - 1;
		}
	/*	else if (size >= (y + y + x) && size <= (y + y + x + x))*/
		else if(size>=(y+y+x)&&size<=(y+y+x+17))
		{
			pNewTail->x = pTail->x - 1;
			pNewTail->y = pTail->y;
		}
	   else if (size >= (y + y + x + 17) && size <= (y + y + x + 15 + 15))
		{
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y + 1;
		}
		else if (size >= (y + y + x + 15 + 15) && size <= (y + y + x + 15 + 15+ 14))
		{
			pNewTail->x = pTail->x+1;
			pNewTail->y = pTail->y;
		}
		else if (size >= (y + y + x + 15 + 15+14) && size <= (y + y + x + 15 + 15 + 14+10))
		{
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y-1;
		}
		else if (size >= (y + y + x + 15 + 15+14+10) && size <= (y + y + x + 15 + 15+14+10+11))
		{
			pNewTail->x = pTail->x -1;
			pNewTail->y = pTail->y;
		}
		else if (size >= (y + y + x + 15 + 15 + 14 + 10+11) && size <= (y + y + x + 15 + 15 + 14 + 10 + 11+7))
		{
			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y + 1;
		}
		else if (size >= (y + y + x + 15 + 15 + 14 + 10+11+7) && size <= (y + y + x + 15 + 15 + 14 + 10 +11+7+2+6))
		{
			pNewTail->x = pTail->x + 1;
			pNewTail->y = pTail->y;
		}
	  else if (size >= (y + y + x + 15 + 15 + 14 + 10 + 11 + 7+2+6) && size <= (y + y + x + 15 + 15 + 14 + 10 + 11 + 7 + 2+6+2))
		{
		  pNewTail->x = pTail->x;
		  pNewTail->y = pTail->y - 1;
		}
	  else if (size == (y + y + x + 15 + 15 + 14 + 10 + 11 + 7 + 2 + 6 + 2 + 1))
	  {
		  pNewTail->x = pTail->x - 1;
		  pNewTail->y = pTail->y;
	  }
	  
		ListPushBack(list_track, pNewTail);
	}
	

}


//������ʯ
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
	int ratio_y		        // �������Χ�ڻ��ж���Ч��
)
{
	if (type == TRACK_STONE)
	{
		LPSTONE stone;
		stone = (LPSTONE)malloc(sizeof(STONE));
		stone->type = type;
		stone->x = x;//�͹����ʼ������ͬ
		stone->y = y;
		stone->dir = dir;
		stone->y_step = y_step;
		stone->state = state;
		stone->color = color;
		stone->power = power;
		stone->ratio_x = ratio_x;
		stone->ratio_y = ratio_y;


		ListPushFront(list_stones, stone);//�������ɵı�ʯ��������������ǰ�� ����ʵ����������ָо�
		return stone;
	}
	else//����������³��ı�ʯ
	{
		LPSTONE stone;
		stone = (LPSTONE)malloc(sizeof(STONE));
		stone->type = type;
		stone->x = x;//�͹����ʼ������ͬ
		stone->y = y;
		stone->dir = dir;
		stone->y_step = y_step;
		stone->state = state;
		stone->color = color;
		stone->power = power;
		stone->ratio_x = ratio_x;
		stone->ratio_y = ratio_y;

		return stone;


	}


}
void TrackStoneMove()//���ж�ǰ���Ƿ����򣬵���ʱд��д����������
{

	int size = get_stone_num();

	for (int i = 0;i < size;i++)
	{
		((LPSTONE)ListGetAt(list_stones, i))->x = ((PGAME_COORD)ListGetAt(list_track, i + 1))->x;
		((LPSTONE)ListGetAt(list_stones, i))->y = ((PGAME_COORD)ListGetAt(list_track, i + 1))->y;
	}
	
	return;
}
void FrogStoneMove(LPSTONE stone)
{
	int x = abs(7 * CELL_PIXEL + 50 - theMouseY);
	int y = abs(18 * CELL_PIXEL + 50 - theMouseX);
	double dir = (y*1.0) / (x*1.0);//����б��  һ��Ҫע��ϸ�ڣ�����������������������������Ȼ��ֻ����ô45�ȡ�������
	if (theMouseX <18 * CELL_PIXEL + 50 && theMouseY <7 * CELL_PIXEL + 50)//���ϲ���
	{
		stone->y -= stone->y_step;
		stone->x -= (int)(stone->y_step*dir);
	}
	else if (theMouseX <= 18 * CELL_PIXEL + 50 && theMouseY >= 7 * CELL_PIXEL + 50)//���²���
	{
		stone->y += stone->y_step;
		stone->x -= (int)(stone->y_step*dir);

	}
	else if (theMouseX > 18 * CELL_PIXEL + 50 && theMouseY <7 * CELL_PIXEL + 50)//���ϲ���
	{
		stone->y -= stone->y_step;
		stone->x += (int)(stone->y_step*dir);
	}
	else if (theMouseX >= 18 * CELL_PIXEL + 50 && theMouseY >= 7 * CELL_PIXEL + 50)//���²���
	{
		stone->y += stone->y_step;
		stone->x += (int)(stone->y_step*dir);
	}
	
}


void stones_destory()
{
	ListClearAndFree(list_stones);
	list_stones = 0;
}
void destory_stone_at(unsigned int i)
{
	LPSTONE S = ListDeleteAt(list_stones, i);
	free(S);
}

unsigned int get_stone_num()
{
	return ListSize(list_stones);
}
LPSTONE get_stone_at(unsigned int i)
{
	return ListGetAt(list_stones, i);
}

unsigned int get_stone_x(LPSTONE stone)
{
	return stone->x;
}
unsigned int get_stone_y(LPSTONE stone)
{
	return stone->y;
}

void stone_be_hit(LPSTONE stone)//����״̬����
{
	stone->state = STONE_HIT;
}
void be_hit(LPSTONE frog_stone)//��Ǻ���
{
	LPSTONE track_stone;
	int flag = 0;//������¼��stone�򵽵ı�ʯ�������е����к�  

	for (int i = 0;i < get_stone_num();i++)
	{
		track_stone = ListGetAt(list_stones, i);
		if ((track_stone->x * 23 + 50 + track_stone->ratio_x >= frog_stone->x * 23 + 50 &&
			track_stone->x * 23 + 50 - track_stone->ratio_x <= frog_stone->x * 23 + 50) && (
				track_stone->y * 23 + 50 + track_stone->ratio_y >= frog_stone->y * 23 + 50 &&
				track_stone->y * 23 + 50 - track_stone->ratio_y <= frog_stone->y * 23 + 50))
		{
			flag = i;
			//track_stone = ((LPSTONE)(ListGetAt(list_stones, flag)));//�����еı�ʯ
			if (track_stone->color == frog_stone->color)//���������������³��ı�ʯ��ɫ��ͬ�Ļ� ����ΪHIT
			{
				int j = 1;
				stone_be_hit(track_stone);
				while (((LPSTONE)(ListGetAt(list_stones, flag + j)))->color == frog_stone->color)
				{
					stone_be_hit(((LPSTONE)(ListGetAt(list_stones, flag + j))));
					j++;
				}
				while (((LPSTONE)(ListGetAt(list_stones, flag - j)))->color == frog_stone->color)
				{
					stone_be_hit(((LPSTONE)(ListGetAt(list_stones, flag - j))));
					j--;
				}

			}
			else
			{
			};
			/*	else
					track_stone->state = STONE_MOVED;*/
		}
		else
		{
		};
		/*else
		{
			track_stone->state = STONE_MOVED;
		}*/
	}



	//else
	//{
	//	track_stone->state = STONE_MOVED;
	//}
}



void destory_stone_by_state()
{
	unsigned 	int num = get_stone_num();
	for (unsigned int i = 0;i < num;i++)
	{
		if (((LPSTONE)(ListGetAt(list_stones, i)))->state == STONE_HIT)
			destory_stone_at(i);
		else
		{
			//ListInsertAt(list_stones, i, stone);
		}
	}
}
DWORD get_sorce()
{
	return dwSorce;
}


unsigned int get_track_num()
{
	return ListSize(list_track);
}


PGAME_COORD GetTrackTail()
{
	return (PGAME_COORD)ListGetAt(list_track, get_track_num() - 1);
}
PGAME_COORD GetTrackAt(int n)//���ڵ���track������
{
	if (n < ListSize(list_track))
		return (PGAME_COORD)ListGetAt(list_track, n);
	else
		return NULL;

}
void destory()
{
	ListDistoryAndFree(list_stones);
	ListDistoryAndFree(list_track);
	return;
}



