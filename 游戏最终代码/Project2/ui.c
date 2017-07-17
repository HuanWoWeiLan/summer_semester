#include<Windows.h>
#include<math.h>
#include"ZuMa.h"
extern LPSTONE stone;
#define BITMAP_FILE	"frog.bmp"

#define CELL_PIXEL 23
#define TRACK_STONE__TIMER_ELAPSE 400// 宝石在轨道上移动的速度
#define FROG_STONE__TIMER_ELAPSE 100// 青蛙宝石移动的速度

#define TRACK_STONE_TIMER_ID 0
#define FROG_STONE_TIMER_ID  1
// 计时器到时间隔
DWORD TrackStoneTimerElapse;
DWORD FrogStoneTimerElapse;

LPSTONE stone = NULL;//用于创建时传给timer函数
#define YELLOW RGB(255,255,0)
#define ORANGE RGB/*(255,165,0) */   	(	255 ,140, 105)
#define BLUE RGB/*(0,191,255)*/		(152, 245, 255)
#define PINK RGB/*(255,105,180)*/(	221, 160, 221)

int counter = 0;//控制游戏暂停和继续


POINT ptfrog;//青蛙的坐标

			 /********************全局变量****************************/
HBITMAP hbmpfrog;
HBITMAP hbmpsnake;
HINSTANCE hinst;
RECT rectBoundary;

/***************************函数声明*******************************************/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

void TrackStoneCreate(HWND hwnd, DWORD dwInitTimerElapse);
void FrogStoneCreate(HWND hwnd, DWORD dwInitTimerElapse);
void FrogCreate(HWND hwnd);

void GamePaint(HWND hwnd);
void FrogStonePaint(HWND hwnd);
void MenuPaint(HWND hwnd);
void TrackStoneTimer(HWND hwnd);
void FrogStoneTimer(HWND hwnd);

void GetPointOfMouse(HWND hwnd);//获取鼠标坐标 用来计算斜率

/**********************************************************************************/

int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{

	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	BOOL fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = "MainMenu";
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
		"+++++++++++++++ZuMa Game+++++++++++++",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);

	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	switch (msg)
	{
	case WM_CREATE:
		
		create_track(20, 15);//20为轨道x，15为y
		FrogCreate(hwnd);
		TrackStoneCreate(hwnd, TRACK_STONE__TIMER_ELAPSE);

		break;
	case WM_PAINT:
		if (stone == NULL)
			GamePaint(hwnd);
		else
		{
			GamePaint(hwnd);
			FrogStonePaint(hwnd);
		}

		//MenuPaint(hwnd);
		break;
	case WM_DESTROY:
		destory();
		ExitProcess(0);
		break;
	case WM_TIMER://当计时器到时以后
		switch (wParam)
		{

		case TRACK_STONE_TIMER_ID:
			TrackStoneTimer(hwnd);
			GamePaint(hwnd);
			
			
			break;
		case FROG_STONE_TIMER_ID:
			FrogStoneTimer(hwnd);
			FrogStonePaint(hwnd);
			break;
		}
		break;


	case WM_LBUTTONDOWN://按下鼠标左键时,创建这个宝石 让玩游戏的人看见下一个宝石是什么颜色
		//但此时不设置计时器 为的是让宝石不发射

		FrogStoneCreate(hwnd, FROG_STONE__TIMER_ELAPSE);//青蛙开始创建宝石 并且吐出来 计时器一到就重绘一下
		FrogStonePaint(hwnd);
		KillTimer(hwnd, FROG_STONE_TIMER_ID);

		break;
	case WM_RBUTTONDOWN://当鼠标点击右键的时候，鼠标的坐标确定 斜率计算 宝石发射
		GetPointOfMouse(hwnd);
		SetTimer(hwnd, FROG_STONE_TIMER_ID, FROG_STONE__TIMER_ELAPSE, NULL);//给青蛙宝石设定计时器



		break;
	}

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);

}



void TrackStoneCreate(HWND hwnd, DWORD dwInitTimerElapse)//创建轨道上的宝石 并让其沿轨道移动
{
	srand((unsigned)time(NULL));
	int Color = rand() % 3;//随机产生 0,1,2,3 枚举类型的四种颜色
	list_stones = ListCreate(NULL);
	create_stone(TRACK_STONE, 8, 1, 0, 0, STONE_MOVED, Color, 0,1,1);//一个格子是23 所以范围就是12差不多
	TrackStoneTimerElapse = dwInitTimerElapse;
	SetTimer(hwnd, TRACK_STONE_TIMER_ID, TrackStoneTimerElapse, NULL);

}
void FrogCreate(HWND hwnd)
{

	hbmpfrog = LoadImage(NULL, "frog.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

}
void FrogStoneCreate(HWND hwnd, DWORD dwInitTimerElapse)
{
	srand((unsigned)time(NULL));
	int Color = rand() % 4;//随机产生 0,1,2,3 枚举类型的四种颜色

						   //让青蛙吐出一个宝石
						   //计算斜率
	//double dir =( (fabs(7 * CELL_PIXEL + 50 - theMouseY))*1.0) / (fabs(18 * CELL_PIXEL + 50 - theMouseX));//计算斜率

	stone = create_stone(FROG_STONE, 18, 7, dir, 2, STONE_MOVED, Color, 0, 0, 0);

	//FrogStoneTimerElapse = dwInitTimerElapse;
	//SetTimer(hwnd, FROG_STONE_TIMER_ID, FrogStoneTimerElapse, NULL);//给青蛙宝石设定计时器


}
void GamePaint(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	HDC hdc, hdcMem, hdcBitmapSrc;
	HBITMAP hBitmap, hbmpOldFrog;

	
	BITMAP bmp;

	hdc = GetDC(hwnd);
	hdcMem = CreateCompatibleDC(hdc);

	hBitmap = CreateCompatibleBitmap(hdc, // 不能是hdcMem，否则会变成黑白2色位图
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcMem, hBitmap);

	hdcBitmapSrc = CreateCompatibleDC(hdc);
	hbmpOldFrog=(HBITMAP)SelectObject(hdcBitmapSrc, hbmpfrog);
	GetObject(hbmpfrog, sizeof(BITMAP), &bmp);

	
		///////////////////////////////////////////////////////////////////////
	//画填充背景
	FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
////////////////////////////////////画青蛙/////////////////////////
	StretchBlt(hdcMem,
		17*23+50,6*23+50,//18 * 23 + 50, 7 * 23 + 50,
		74, 81,
		hdcBitmapSrc,
		0, 0, bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);


	///////////////////////////////////////////////画轨道////////////////////////////////////////////////////

	HPEN hpen2;
	HBRUSH hbrush2;

	hpen2 = CreatePen(PS_SOLID, 3, RGB(255 ,174 ,185));
	hbrush2 = CreateSolidBrush(RGB(255,255,255/*144, 238, 144*/));
	PGAME_COORD pTrack;

	SelectObject(hdcMem, hpen2);
	SelectObject(hdcMem, hbrush2);

	int i, track_size;
	track_size = get_track_num();

	for (i = 0; i <track_size; i++)
	{
		pTrack = (PGAME_COORD)GetTrackAt(i);
		if (i < track_size-1)
		{
			Rectangle(hdcMem,
				pTrack->x * CELL_PIXEL + 50,
				pTrack->y * CELL_PIXEL + 50,
				(pTrack->x + 1)*CELL_PIXEL + 50,
				(pTrack->y + 1)*CELL_PIXEL + 50);
		}
		else
			Ellipse(hdcMem,
				pTrack->x * CELL_PIXEL + 50,
				pTrack->y * CELL_PIXEL + 50,
				(pTrack->x + 1)*CELL_PIXEL + 50,
				(pTrack->y + 1)*CELL_PIXEL + 50);

	}



	///////////////////////////////////////////////////////////////画轨道上的石头/////////////////////////////////////////////////////
	HPEN hpen3, hpen4, hpen5, hpen6, hpen7;
	HBRUSH hbrush3, hbrush4, hbrush5, hbrush6, hbrush7;
	LPSTONE pStone;

		hpen3 = CreatePen(PS_SOLID, 1, YELLOW);  hbrush3 = CreateSolidBrush(YELLOW);
		hpen4= CreatePen(PS_SOLID, 1, ORANGE); hbrush4 = CreateSolidBrush(ORANGE);
		hpen5 = CreatePen(PS_SOLID, 1, BLUE); hbrush5 = CreateSolidBrush(BLUE);
		hpen6= CreatePen(PS_SOLID, 1, PINK); hbrush6= CreateSolidBrush(PINK);
		//
	hpen7 = CreatePen(PS_SOLID, 3, RGB(255, 174, 185));
		hbrush7 = CreateSolidBrush(RGB(255, 255, 255/*144, 238, 144*/));
		//hpen7= CreatePen(PS_SOLID, 1, 0); hbrush7 = CreateSolidBrush(0);


	int j, stone_size;
	stone_size = get_stone_num();

	for (j = 0; j < stone_size; j++)
	{
		pStone = (LPSTONE)get_stone_at(j);
		int Color = pStone->color;

		if (pStone->state != STONE_HIT)
		{
			switch (Color)
			{
			case 0:
				SelectObject(hdcMem, hpen3);
				SelectObject(hdcMem, hbrush3);
				break;
			case 1:
				SelectObject(hdcMem, hpen4);
				SelectObject(hdcMem, hbrush4);
				break;
			case 2:
				SelectObject(hdcMem, hpen5);
				SelectObject(hdcMem, hbrush5);
				break;
			case 3:
				SelectObject(hdcMem, hpen6);
				SelectObject(hdcMem, hbrush6);
				break;
			default:
				SelectObject(hdcMem, hpen6);
				SelectObject(hdcMem, hbrush6);
				break;
			}
			Ellipse(hdcMem,
				pStone->x * CELL_PIXEL + 50,
				pStone->y * CELL_PIXEL + 50,
				(pStone->x + 1)*CELL_PIXEL + 50,
				(pStone->y + 1)*CELL_PIXEL + 50);

		}
	
		 else
		{
			SelectObject(hdcMem, hpen7);
			SelectObject(hdcMem, hbrush7);

			Rectangle(hdcMem,
				pStone->x * CELL_PIXEL + 50,
				pStone->y * CELL_PIXEL + 50,
				(pStone->x + 1)*CELL_PIXEL + 50,
				(pStone->y + 1)*CELL_PIXEL + 50);

		}



			//if(pStone->state==STONE_MOVED)//判断stone的类型  来确定是否要重绘
		
	
	}
	/////////////////////////////////////////////////写字/////////////////////////////////////////////////
	HFONT hFont, hOldFont;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//调用api函数


	// 创建了一个字体对象
	hFont = CreateFont(100, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Adobe Gothic Std B"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcMem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "呱", "%d", 6);
		// 设置输出颜色
		SetTextColor(hdcMem, PINK);
		// 输出字符串。
		TextOut(hdcMem, 80,100,
			szSourceInfo, lstrlen(szSourceInfo));
		TextOut(hdcMem, 750,100,
			szSourceInfo, lstrlen(szSourceInfo));
		TextOut(hdcMem, 0, 4 + 25 * 2,
			szSourceInfo, lstrlen(szSourceInfo));
	


		TextOut(hdcMem, 30*24, 1 + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		TextOut(hdcMem, 66, 280,
			szSourceInfo, lstrlen(szSourceInfo));
		TextOut(hdcMem, 190,455,
		szSourceInfo, lstrlen(szSourceInfo));
		TextOut(hdcMem, 850,200,
		szSourceInfo, lstrlen(szSourceInfo));
	TextOut(hdcMem, 780,300,
			szSourceInfo, lstrlen(szSourceInfo));
	TextOut(hdcMem, 400,600,
			szSourceInfo, lstrlen(szSourceInfo));
	TextOut(hdcMem, 160, 180,
		szSourceInfo, lstrlen(szSourceInfo));

		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcMem, hOldFont);
	}
	// 在内存DC中画完，一次输出的窗口DC上。

	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcMem, 0, 0, SRCCOPY);

SelectObject(hdcBitmapSrc, hbmpOldFrog);
	//SelectObject(hdcBitmapSrc1, hbmpOld);

	DeleteObject(hFont);
	DeleteObject(hOldFont);
	DeleteObject(hpen2);
	DeleteObject(hbrush2);
	DeleteObject(hpen3);
	DeleteObject(hbrush3);
	DeleteObject(hpen4);
	DeleteObject(hbrush4);
	DeleteObject(hpen5);
	DeleteObject(hbrush5);
	DeleteObject(hpen6);
	DeleteObject(hbrush6);
	DeleteObject(hpen7);
	DeleteObject(hbrush7);
	ReleaseDC(hwnd,hdcMem);
	ReleaseDC(hwnd, hdcBitmapSrc);
	ReleaseDC(hwnd, hdc);

}
//void MenuPaint(HWND hwnd)
//{
//
//	HBITMAP hbmpMenu = LoadImage(NULL, "tree.bmp",
//		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//	RECT rect;
//	GetClientRect(hwnd, &rect);
//
//	HDC hdc, hdcMem, hdcBitmapSrc;
//	HBITMAP hBitmap, hbmpOldFrog;
//
//
//	BITMAP bmp;
//
//	hdc = GetDC(hwnd);
//	hdcMem = CreateCompatibleDC(hdc);
//
//	hBitmap = CreateCompatibleBitmap(hdc, // 不能是hdcMem，否则会变成黑白2色位图
//		rect.right - rect.left, rect.bottom - rect.top);
//
//	SelectObject(hdcMem, hBitmap);
//
//	hdcBitmapSrc = CreateCompatibleDC(hdc);
//	hbmpOldFrog = (HBITMAP)SelectObject(hdcBitmapSrc, hbmpMenu);
//	GetObject(hbmpMenu, sizeof(BITMAP), &bmp);
//
//
//	///////////////////////////////////////////////////////////////////////
//	//画填充背景
//	FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
//	////////////////////////////////////画青蛙/////////////////////////
//	StretchBlt(hdcMem,
//		17 * 23 + 50, 6 * 23 + 50,//18 * 23 + 50, 7 * 23 + 50,
//		74, 81,
//		hdcBitmapSrc,
//		0, 0, bmp.bmWidth, bmp.bmHeight,
//		SRCCOPY);
//
//	ReleaseDC(hwnd, hdcMem);
//	ReleaseDC(hwnd, hdcBitmapSrc);
//	ReleaseDC(hwnd, hdc);
//
//
//
//}

void FrogStonePaint(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	GamePaint(hwnd);//重绘

	HPEN hpen;
	HBRUSH hbrush;

	LPSTONE pStone = stone;//等于那个全局变量 stone  即发射的宝石
						   ////////////////////////////////////////////////////////////////使发射的球的颜色随机/////////////////////////////////////////////////////
	int Color = pStone->color;
	//随机产生 0,1,2,3 枚举类型的四种颜色
	switch (Color)
	{
	case 0:
		hpen = CreatePen(PS_SOLID, 1, YELLOW);
		hbrush = CreateSolidBrush(YELLOW);
		break;

	case 1:
		hpen = CreatePen(PS_SOLID, 1, ORANGE);
		hbrush = CreateSolidBrush(ORANGE);
		break;
	case 2:
		hpen = CreatePen(PS_SOLID, 1, BLUE);
		hbrush = CreateSolidBrush(BLUE);
		break;
	case 3:
		hpen = CreatePen(PS_SOLID, 1, PINK);
		hbrush = CreateSolidBrush(PINK);
		break;
	default:
		hpen = CreatePen(PS_SOLID, 1, PINK);
		hbrush = CreateSolidBrush(PINK);
		break;
	}

	SelectObject(hdc, hpen);
	SelectObject(hdc, hbrush);
	Ellipse(hdc,
		pStone->x * CELL_PIXEL + 50,
		pStone->y * CELL_PIXEL + 50,
		(pStone->x + 1)*CELL_PIXEL + 50,
		(pStone->y + 1)*CELL_PIXEL + 50);
	

	DeleteObject(hpen);
	DeleteObject(hbrush);
	ReleaseDC(hwnd, hdc);
}
void TrackStoneTimer(HWND hwnd)
{
	//如果到头了的话 就关闭计时器
	if (get_stone_num() > MAX_TRACK_STONE)
	{
		KillTimer(hwnd, TRACK_STONE_TIMER_ID);
		MessageBox(hwnd, "Game Over!!!!!\n 球被我吃掉啦哈哈哈哈", "FROG", MB_DEFBUTTON1);
		exit(1);
	}
	/////啦啦啦啦
	//destory_stone_by_state();

	if (counter<= 70||counter>110)//一次暂停的机会
	{
		TrackStoneMove();
		srand((unsigned)time(NULL));


		int Color = rand() % 4;//随机产生 0,1,2,3 枚举类型的四种颜色

		int size = get_stone_num();
		//int x = GetTrackAt(size)->x;
		//int y = GetTrackAt(size)->y;
		create_stone(TRACK_STONE, 8, 1, 0, 0, STONE_MOVED, Color, 0, 12, 12);
		
	}
	//else if (counter == 101)
	//{
	//	MessageBox(hwnd, "抓紧机会！！！！球不动啦！！！", "暂停时间~~~~", MB_DEFBUTTON1);
	//}
	else if(counter>70&&counter<110)
	{
		//FontPaint(hwnd);
	}

	counter++;

	//if (stone != NULL)
	//	be_hit(stone);

}//ok
void FrogStoneTimer(HWND hwnd)//x,y代表鼠标点下左键之后 在窗口中的坐标
{
	////new
	FrogStoneMove(stone);
	//青蛙石头每移动一次判断并标记
	be_hit(stone);
	//destory_stone_by_state();
}
void GetPointOfMouse(HWND hwnd)//用来获得鼠标坐标的函数 单独拿出来写，设置全局变量theMouseX和theMouseY
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	//GetWindowRect(hwnd, &rect);
	theMouseX = point.x;
	theMouseY = point.y;

}








