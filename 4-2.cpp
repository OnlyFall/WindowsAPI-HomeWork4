#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "resource1.h"
#define location 550
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");



struct Ball
{
	int x, y;
};

struct Brick {
	int xpos, ypos;
	int hitCount;
};

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //프로그램 내에서 보이는 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 
	WndClass.hInstance = hinstance; //현재 실행되고 있는 객체의 핸들
	WndClass.lpfnWndProc = WndProc; //프로시저함수의 이름 
	WndClass.lpszClassName = lpszClass; //윈도우 클래스 내용의 이름 
	WndClass.lpszMenuName = NULL; // 메뉴바 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 출력 스타일
											  // 여기까지 WNDCLASS구조체의 변수들에 값을 대입

	RegisterClass(&WndClass); // 윈도우 클래스를 운영체제에 등록

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1420, 1050, NULL, (HMENU)LoadMenu(hinstance, MAKEINTRESOURCE(IDR_MENU3)), hinstance, NULL);
	//CreateWindow(클래스 이름,타이틀 이름,윈도우 스타일, 윈도우 위치좌표x , y,윈도우 가로크기, 윈도우 세로크기, 부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보(NULL))
	ShowWindow(hWnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hWnd); //OS에 WM_PAINT메시지 전송
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//윈도우 프로시저에서 PostQuitMessage()를 호출할때 종료됨.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//메시지루프를 돌림.
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
{
	static PAINTSTRUCT ps;
	static HDC hdc;
	static HPEN hpen, oldpen;
	static HBRUSH hBrush, oldBrush;
	static int angle;
	static Ball ball;
	static Brick brick[4][10]; // 벽돌 2*10개
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	static char buf[10];
	static int xpos = 0, ypos = 600; // 공을 튀기는 블럭
	static int checkX, checkY;
	static int mouseX;
	static int a;
	static int speed = 50;
	static bool select = FALSE;
	static int bound = 0;
	static int count = 0;
	static int blockCount = 2;
	srand(unsigned(time));
	switch (iMessage) //메시지의 번호
	{
	case WM_CREATE:
		/*for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 10; i++)
			{
				brick[j][i].xpos = i * 100;
				brick[j][i].ypos = j * 50;
				brick[j][i].hitCount = 2;
			}
		}*/
		ball.y = 580;
		ball.x = 0;
		
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (ball.y < 0)
			{
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 2, speed, NULL);
			}
			if (a > 1400)
			{
				ball.x -= angle;
			}
			else
			{
				ball.x += angle;
			}
			if (ball.x < 0)
			{
				a = 0;
			}
			a += angle;
			ball.y -= 10;
			break;
		case 2:
			if (a > 1400)
			{
				ball.x -= angle;
			}
			else
			{
				ball.x += angle;
			}
			if (ball.x < 0)
			{
				a = 0;
			}
			a += angle;
			ball.y += 10;
			break;

		case 3:
			if (count % 2 == 0) {
				for (int i = 0; i < blockCount; i++) {
					for (int j = 0; j < 10; j++)
						brick[i][j].xpos += 10;
				}
				if (brick[1][9].xpos + 100 == 1400)
					count++;
			}
			else if (count % 2 == 1) {
				for (int i = 0; i < blockCount; i++) {
					for (int j = 0; j < 10; j++)
						brick[i][j].xpos -= 10;
				}
				if (brick[1][0].xpos == 0)
					count++;
			}
			break;
		}

		for (int j = 0; j < blockCount; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (ball.y - brick[j][i].ypos < 75 && ball.x >= brick[j][i].xpos&&ball.x < brick[j][i].xpos + 100 && brick[j][i].hitCount != 0) // 공과 블록의 충돌체크
				{
					brick[j][i].hitCount -= 1;
					brick[j][i].ypos += 50;
					ball.y += 100;
					SetTimer(hWnd, 2, speed, NULL);
					KillTimer(hWnd, 1);
				}
			}
		}
		if (ypos - ball.y < 20 && ball.x >= xpos && xpos + 200>ball.x) // 바와 공의 충돌체크 바와 공사이의 거리가 20보다 작으면 충돌한거임.
		{
			KillTimer(hWnd, 2);
			SetTimer(hWnd, 1, speed, NULL);
		}
		if (ball.y > 700)
		{
			PostQuitMessage(0);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_COMMAND:
		switch (wParam) {
		case ID_GAME_START:
			SetTimer(hWnd, 1, speed, NULL);	
			SetTimer(hWnd, 3, 30, NULL);
			break;

		case ID_GAME_END:
			PostQuitMessage(0);
			break;

		case ID_SPEED_FAST:
			KillTimer(hWnd, 1);
			speed =5;
			SetTimer(hWnd, 1, speed, NULL);
			break;

		case ID_SPEED_MEDIUM:
			KillTimer(hWnd, 1);
			speed = 50;
			SetTimer(hWnd, 1, speed, NULL);
			break;

		case ID_SPEED_SLOW:
			KillTimer(hWnd, 1);
			speed = 1000;
			SetTimer(hWnd, 1, speed, NULL);
			break;

		case ID_BLOCK_5:
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 10; i++)
				{
					brick[j][i].xpos = i * 100;
					brick[j][i].ypos = j * 50;
					brick[j][i].hitCount = 2;
				}
			}
			blockCount = 2;
			break;

		case ID_BLOCK_6:
			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 10; i++)
				{
					brick[j][i].xpos = i * 100;
					brick[j][i].ypos = j * 50;
					brick[j][i].hitCount = 2;
				}
			}
			blockCount = 3;
			break;

		case ID_BLOCK_7:
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 10; i++)
				{
					brick[j][i].xpos = i * 100;
					brick[j][i].ypos = j * 50;
					brick[j][i].hitCount = 2;
				}
				blockCount = 4;
			}
			break;
			// 벽돌 단수를 추가합시다.
		}
		break;
	case WM_KEYDOWN:

		break;
	case WM_CHAR:
		if (wParam == '-')
		{
			speed -= 5;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		mouseX = LOWORD(lParam);
		xpos = mouseX;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		angle = rand() % 5 + 2;
		hdc = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, 1420, 1050);
		hbmMemOld = (HBITMAP)SelectObject(memDC, hBitmap);
		SelectObject(memDC, (HBITMAP)hBitmap);
		PatBlt(memDC, 0, 0, 1420, 1050, WHITENESS);

		Rectangle(memDC, 0, 0, 1400, 700);
		Rectangle(memDC, xpos, ypos, xpos + 200, ypos + 30);
		for (int j = 0; j < blockCount; j++) {
			for (int i = 0; i < 10; i++)
			{
				if (brick[j][i].hitCount == 2) {
					hBrush = CreateSolidBrush(RGB(255, 255, 255));
					oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
					Rectangle(memDC, brick[j][i].xpos, brick[j][i].ypos, brick[j][i].xpos + 100, brick[j][i].ypos + 50);
					DeleteObject(oldBrush);
				}
				else if (brick[j][i].hitCount == 1)
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
					Rectangle(memDC, brick[j][i].xpos, brick[j][i].ypos, brick[j][i].xpos + 100, brick[j][i].ypos + 50);
					DeleteObject(oldBrush);
				}
			}
		}
		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		Ellipse(memDC, ball.x - 10, ball.y - 10, ball.x + 10, ball.y + 10);

		BitBlt(hdc, 0, 0, 1420, 1050, memDC, 0, 0, SRCCOPY);

		DeleteObject(oldBrush);
		DeleteDC(memDC);
		DeleteObject(hbmMemOld);
		DeleteDC(hdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}