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

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h�� �ǹ�? = �ڵ�
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //���α׷� ������ ���̴� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ 
	WndClass.hInstance = hinstance; //���� ����ǰ� �ִ� ��ü�� �ڵ�
	WndClass.lpfnWndProc = WndProc; //���ν����Լ��� �̸� 
	WndClass.lpszClassName = lpszClass; //������ Ŭ���� ������ �̸� 
	WndClass.lpszMenuName = NULL; // �޴��� �̸�
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //������ ��� ��Ÿ��
											  // ������� WNDCLASS����ü�� �����鿡 ���� ����

	RegisterClass(&WndClass); // ������ Ŭ������ �ü���� ���

	hWnd = CreateWindow(lpszClass, "Windows Program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1420, 1050, NULL, (HMENU)LoadMenu(hinstance, MAKEINTRESOURCE(IDR_MENU3)), hinstance, NULL);
	//CreateWindow(Ŭ���� �̸�,Ÿ��Ʋ �̸�,������ ��Ÿ��, ������ ��ġ��ǥx , y,������ ����ũ��, ������ ����ũ��, �θ� ������ �ڵ�, �޴� �ڵ�, �������α׷� �ν��Ͻ�, ���� ������ ����(NULL))
	ShowWindow(hWnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hWnd); //OS�� WM_PAINT�޽��� ����
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//������ ���ν������� PostQuitMessage()�� ȣ���Ҷ� �����.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//�޽��������� ����.
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	static PAINTSTRUCT ps;
	static HDC hdc;
	static HPEN hpen, oldpen;
	static HBRUSH hBrush, oldBrush;
	static int angle;
	static Ball ball;
	static Brick brick[4][10]; // ���� 2*10��
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	static char buf[10];
	static int xpos = 0, ypos = 600; // ���� Ƣ��� ��
	static int checkX, checkY;
	static int mouseX;
	static int a;
	static int speed = 50;
	static bool select = FALSE;
	static int bound = 0;
	static int count = 0;
	static int blockCount = 2;
	srand(unsigned(time));
	switch (iMessage) //�޽����� ��ȣ
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
				if (ball.y - brick[j][i].ypos < 75 && ball.x >= brick[j][i].xpos&&ball.x < brick[j][i].xpos + 100 && brick[j][i].hitCount != 0) // ���� ����� �浹üũ
				{
					brick[j][i].hitCount -= 1;
					brick[j][i].ypos += 50;
					ball.y += 100;
					SetTimer(hWnd, 2, speed, NULL);
					KillTimer(hWnd, 1);
				}
			}
		}
		if (ypos - ball.y < 20 && ball.x >= xpos && xpos + 200>ball.x) // �ٿ� ���� �浹üũ �ٿ� �������� �Ÿ��� 20���� ������ �浹�Ѱ���.
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
			// ���� �ܼ��� �߰��սô�.
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