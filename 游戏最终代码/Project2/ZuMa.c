#include"ZuMa.h"
#include"LinkList.h"
#include<time.h>

DWORD dwSorce = 0;

extern LPSTONE stone;



//创建轨道
void create_track(
	int x,   //长
	int y   //宽
			//TRACK_COLOR color
)
{
	PGAME_COORD p;

	int i;

	list_track = ListCreate(NULL);

	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));

	// 轨道头部的初始位置；
	p->x = 8;
	p->y = 1;
	ListPushFront(list_track, p);

	for (int i = 0;i < MAX_TRACK_STONE;i++)
	{
		PGAME_COORD pNewTail = (PGAME_COORD)malloc(sizeof(GAME_COORD));
		int size = get_track_num();
		PGAME_COORD pTail = GetTrackTail();
		if (size < y)//左边界
		{

			pNewTail->x = pTail->x;
			pNewTail->y = pTail->y + 1;
		}
		else if (size >= y&&size < (x + y))//下边界
		{
			pNewTail->x = pTail->x + 1;
			pNewTail->y = pTail->y;
		}
		else if (size >= (x + y) && size <= (y + y + x))//右边界
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


//创建宝石
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
	int ratio_y		        // 在这个范围内击中都有效。
)
{
	if (type == TRACK_STONE)
	{
		LPSTONE stone;
		stone = (LPSTONE)malloc(sizeof(STONE));
		stone->type = type;
		stone->x = x;//和轨道初始坐标相同
		stone->y = y;
		stone->dir = dir;
		stone->y_step = y_step;
		stone->state = state;
		stone->color = color;
		stone->power = power;
		stone->ratio_x = ratio_x;
		stone->ratio_y = ratio_y;


		ListPushFront(list_stones, stone);//把新生成的宝石插入在整个链表前面 才能实现祖玛的那种感觉
		return stone;
	}
	else//如果是青蛙吐出的宝石
	{
		LPSTONE stone;
		stone = (LPSTONE)malloc(sizeof(STONE));
		stone->type = type;
		stone->x = x;//和轨道初始坐标相同
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
void TrackStoneMove()//得判断前后是否还有球，但暂时写不写……哈哈哈
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
	double dir = (y*1.0) / (x*1.0);//计算斜率  一定要注意细节！！！！！！！！！！！！！！不然就只有特么45度…………
	if (theMouseX <18 * CELL_PIXEL + 50 && theMouseY <7 * CELL_PIXEL + 50)//左上部分
	{
		stone->y -= stone->y_step;
		stone->x -= (int)(stone->y_step*dir);
	}
	else if (theMouseX <= 18 * CELL_PIXEL + 50 && theMouseY >= 7 * CELL_PIXEL + 50)//左下部分
	{
		stone->y += stone->y_step;
		stone->x -= (int)(stone->y_step*dir);

	}
	else if (theMouseX > 18 * CELL_PIXEL + 50 && theMouseY <7 * CELL_PIXEL + 50)//右上部分
	{
		stone->y -= stone->y_step;
		stone->x += (int)(stone->y_step*dir);
	}
	else if (theMouseX >= 18 * CELL_PIXEL + 50 && theMouseY >= 7 * CELL_PIXEL + 50)//右下部分
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

void stone_be_hit(LPSTONE stone)//设置状态函数
{
	stone->state = STONE_HIT;
}
void be_hit(LPSTONE frog_stone)//标记函数
{
	LPSTONE track_stone;
	int flag = 0;//用来记录与stone打到的宝石在链表中的序列号  

	for (int i = 0;i < get_stone_num();i++)
	{
		track_stone = ListGetAt(list_stones, i);
		if ((track_stone->x * 23 + 50 + track_stone->ratio_x >= frog_stone->x * 23 + 50 &&
			track_stone->x * 23 + 50 - track_stone->ratio_x <= frog_stone->x * 23 + 50) && (
				track_stone->y * 23 + 50 + track_stone->ratio_y >= frog_stone->y * 23 + 50 &&
				track_stone->y * 23 + 50 - track_stone->ratio_y <= frog_stone->y * 23 + 50))
		{
			flag = i;
			//track_stone = ((LPSTONE)(ListGetAt(list_stones, flag)));//被击中的宝石
			if (track_stone->color == frog_stone->color)//如果这个结点和青蛙吐出的宝石颜色相同的话 则标记为HIT
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
PGAME_COORD GetTrackAt(int n)//按节点获得track的坐标
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



