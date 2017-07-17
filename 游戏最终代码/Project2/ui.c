#include<Windows.h>
#include<math.h>
#include"ZuMa.h"
extern LPSTONE stone;
#define BITMAP_FILE	"frog.bmp"

#define CELL_PIXEL 23
#define TRACK_STONE__TIMER_ELAPSE 400// ��ʯ�ڹ�����ƶ����ٶ�
#define FROG_STONE__TIMER_ELAPSE 100// ���ܱ�ʯ�ƶ����ٶ�

#define TRACK_STONE_TIMER_ID 0
#define FROG_STONE_TIMER_ID  1
// ��ʱ����ʱ���
DWORD TrackStoneTimerElapse;
DWORD FrogStoneTimerElapse;

LPSTONE stone = NULL;//���ڴ���ʱ����timer����
#define YELLOW RGB(255,255,0)
#define ORANGE RGB/*(255,165,0) */   	(	255 ,140, 105)
#define BLUE RGB/*(0,191,255)*/		(152, 245, 255)
#define PINK RGB/*(255,105,180)*/(	221, 160, 221)

int counter = 0;//������Ϸ��ͣ�ͼ���


POINT ptfrog;//���ܵ�����

			 /********************ȫ�ֱ���****************************/
HBITMAP hbmpfrog;
HBITMAP hbmpsnake;
HINSTANCE hinst;
RECT rectBoundary;

/***************************��������*******************************************/
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

void GetPointOfMouse(HWND hwnd);//��ȡ������� ��������б��

/**********************************************************************************/

int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{

	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	BOOL fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = "MainMenu";
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
		"+++++++++++++++ZuMa Game+++++++++++++",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwnd);

	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
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
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	switch (msg)
	{
	case WM_CREATE:
		
		create_track(20, 15);//20Ϊ���x��15Ϊy
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
	case WM_TIMER://����ʱ����ʱ�Ժ�
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


	case WM_LBUTTONDOWN://����������ʱ,���������ʯ ������Ϸ���˿�����һ����ʯ��ʲô��ɫ
		//����ʱ�����ü�ʱ�� Ϊ�����ñ�ʯ������

		FrogStoneCreate(hwnd, FROG_STONE__TIMER_ELAPSE);//���ܿ�ʼ������ʯ �����³��� ��ʱ��һ�����ػ�һ��
		FrogStonePaint(hwnd);
		KillTimer(hwnd, FROG_STONE_TIMER_ID);

		break;
	case WM_RBUTTONDOWN://��������Ҽ���ʱ����������ȷ�� б�ʼ��� ��ʯ����
		GetPointOfMouse(hwnd);
		SetTimer(hwnd, FROG_STONE_TIMER_ID, FROG_STONE__TIMER_ELAPSE, NULL);//�����ܱ�ʯ�趨��ʱ��



		break;
	}

	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);

}



void TrackStoneCreate(HWND hwnd, DWORD dwInitTimerElapse)//��������ϵı�ʯ �������ع���ƶ�
{
	srand((unsigned)time(NULL));
	int Color = rand() % 3;//������� 0,1,2,3 ö�����͵�������ɫ
	list_stones = ListCreate(NULL);
	create_stone(TRACK_STONE, 8, 1, 0, 0, STONE_MOVED, Color, 0,1,1);//һ��������23 ���Է�Χ����12���
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
	int Color = rand() % 4;//������� 0,1,2,3 ö�����͵�������ɫ

						   //�������³�һ����ʯ
						   //����б��
	//double dir =( (fabs(7 * CELL_PIXEL + 50 - theMouseY))*1.0) / (fabs(18 * CELL_PIXEL + 50 - theMouseX));//����б��

	stone = create_stone(FROG_STONE, 18, 7, dir, 2, STONE_MOVED, Color, 0, 0, 0);

	//FrogStoneTimerElapse = dwInitTimerElapse;
	//SetTimer(hwnd, FROG_STONE_TIMER_ID, FrogStoneTimerElapse, NULL);//�����ܱ�ʯ�趨��ʱ��


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

	hBitmap = CreateCompatibleBitmap(hdc, // ������hdcMem��������ɺڰ�2ɫλͼ
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcMem, hBitmap);

	hdcBitmapSrc = CreateCompatibleDC(hdc);
	hbmpOldFrog=(HBITMAP)SelectObject(hdcBitmapSrc, hbmpfrog);
	GetObject(hbmpfrog, sizeof(BITMAP), &bmp);

	
		///////////////////////////////////////////////////////////////////////
	//����䱳��
	FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
////////////////////////////////////������/////////////////////////
	StretchBlt(hdcMem,
		17*23+50,6*23+50,//18 * 23 + 50, 7 * 23 + 50,
		74, 81,
		hdcBitmapSrc,
		0, 0, bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);


	///////////////////////////////////////////////�����////////////////////////////////////////////////////

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



	///////////////////////////////////////////////////////////////������ϵ�ʯͷ/////////////////////////////////////////////////////
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



			//if(pStone->state==STONE_MOVED)//�ж�stone������  ��ȷ���Ƿ�Ҫ�ػ�
		
	
	}
	/////////////////////////////////////////////////д��/////////////////////////////////////////////////
	HFONT hFont, hOldFont;
	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);//����api����


	// ������һ���������
	hFont = CreateFont(100, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Adobe Gothic Std B"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcMem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "��", "%d", 6);
		// ���������ɫ
		SetTextColor(hdcMem, PINK);
		// ����ַ�����
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

		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcMem, hOldFont);
	}
	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�

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
//	hBitmap = CreateCompatibleBitmap(hdc, // ������hdcMem��������ɺڰ�2ɫλͼ
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
//	//����䱳��
//	FillRect(hdcMem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
//	////////////////////////////////////������/////////////////////////
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

	GamePaint(hwnd);//�ػ�

	HPEN hpen;
	HBRUSH hbrush;

	LPSTONE pStone = stone;//�����Ǹ�ȫ�ֱ��� stone  ������ı�ʯ
						   ////////////////////////////////////////////////////////////////ʹ����������ɫ���/////////////////////////////////////////////////////
	int Color = pStone->color;
	//������� 0,1,2,3 ö�����͵�������ɫ
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
	//�����ͷ�˵Ļ� �͹رռ�ʱ��
	if (get_stone_num() > MAX_TRACK_STONE)
	{
		KillTimer(hwnd, TRACK_STONE_TIMER_ID);
		MessageBox(hwnd, "Game Over!!!!!\n ���ҳԵ�����������", "FROG", MB_DEFBUTTON1);
		exit(1);
	}
	/////��������
	//destory_stone_by_state();

	if (counter<= 70||counter>110)//һ����ͣ�Ļ���
	{
		TrackStoneMove();
		srand((unsigned)time(NULL));


		int Color = rand() % 4;//������� 0,1,2,3 ö�����͵�������ɫ

		int size = get_stone_num();
		//int x = GetTrackAt(size)->x;
		//int y = GetTrackAt(size)->y;
		create_stone(TRACK_STONE, 8, 1, 0, 0, STONE_MOVED, Color, 0, 12, 12);
		
	}
	//else if (counter == 101)
	//{
	//	MessageBox(hwnd, "ץ�����ᣡ�������򲻶���������", "��ͣʱ��~~~~", MB_DEFBUTTON1);
	//}
	else if(counter>70&&counter<110)
	{
		//FontPaint(hwnd);
	}

	counter++;

	//if (stone != NULL)
	//	be_hit(stone);

}//ok
void FrogStoneTimer(HWND hwnd)//x,y�������������֮�� �ڴ����е�����
{
	////new
	FrogStoneMove(stone);
	//����ʯͷÿ�ƶ�һ���жϲ����
	be_hit(stone);
	//destory_stone_by_state();
}
void GetPointOfMouse(HWND hwnd)//��������������ĺ��� �����ó���д������ȫ�ֱ���theMouseX��theMouseY
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	//GetWindowRect(hwnd, &rect);
	theMouseX = point.x;
	theMouseY = point.y;

}








