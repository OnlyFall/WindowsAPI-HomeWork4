#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

struct Car
{
	int x1, x2;
	int y1, y2;
	int R, G, B;
	int choice;
	int num;
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

	hWnd = CreateWindow(lpszClass, "�λ�", WS_OVERLAPPEDWINDOW, 0, 0, 900, 900, NULL, (HMENU)LoadMenu(hinstance, MAKEINTRESOURCE(IDR_MENU1)), hinstance, NULL);
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
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hpen, oldpen;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	static Car car[8];
	static int YellowC = 0;
	static int check[4] = { 0 };
	static int stop = 1;
	static int speed = 0;
	static int sibal = 0;
	static int checkN = 0;
	static int hi = 0;
	static int RedTraffic = 0;
	static SIZE size;
	static RECT rect;
	static int count[4] = { 0 };
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:

		
		car[0].x1 = -150;
		car[0].y1 = 350;
		car[0].x2 = 0;
		car[0].y2 = 400;
		car[1].x1 = -150;
		car[1].y1 = 350;
		car[1].x2 = 0;
		car[1].y2 = 400;

		car[2].x1 = 900;
		car[2].y1 = 500;
		car[2].x2 = 1050;
		car[2].y2 = 550;
		car[3].x1 = 900;
		car[3].y1 = 500;
		car[3].x2 = 1050;
		car[3].y2 = 550;

		car[4].x1 = 350;
		car[4].y1 = 0;
		car[4].x2 = 400;
		car[4].y2 = -150;
		car[5].x1 = 350;
		car[5].y1 = 0;
		car[5].x2 = 400;
		car[5].y2 = -150;

		car[6].x1 = 500;
		car[6].y1 = 900;
		car[6].x2 = 550;
		car[6].y2 = 1050;
		car[7].x1 = 500;
		car[7].y1 = 900;
		car[7].x2 = 550;
		car[7].y2 = 1050;

		break;
	case WM_KEYDOWN:
		break;

	case WM_COMMAND:
		switch (wParam) {
		case ID_GAME_START:
			SetTimer(hWnd, 1, 10, NULL);
			SetTimer(hWnd, 3, 30, NULL);
			SetTimer(hWnd, 5, 50, NULL);
			SetTimer(hWnd, 7, 70, NULL);
			break;

		case ID_GAME_END:
			PostQuitMessage(0);
			break;

			case ID_SPEED_ACCELERATION:
				speed -= 5;
				break;

		case ID_SPEED_DECELERATION:
			speed += 5;
			break;

		case ID_TRAFFIC_RED:
			if(RedTraffic == 0)
				RedTraffic = 1;		
			break;

		case ID_TRAFFIC_YELLOW:
			SetTimer(hWnd, 9, 1000, NULL);
			break;

		case ID_TRAFFIC_GREEN:
			if (RedTraffic == 1)
			{
				RedTraffic = 0;
				if (hi % 8 == 0)
					SetTimer(hWnd, 1, 10 + speed, NULL);
				else if (hi % 8 == 1)
					SetTimer(hWnd, 3, 30 + speed, NULL);
				else if (hi % 8 == 2)
					SetTimer(hWnd, 5, 50 + speed, NULL);
				else if (hi % 8 == 3)
					SetTimer(hWnd, 7, 70 + speed, NULL);
				else if (hi % 8 == 4)
					SetTimer(hWnd, 2, 10 + speed, NULL);
				else if (hi % 8 == 5)
					SetTimer(hWnd, 4, 30 + speed, NULL);
				else if (hi % 8 == 6)
					SetTimer(hWnd, 6, 50 + speed, NULL);
				else if (hi % 8 == 7)
					SetTimer(hWnd, 7, 70 + speed, NULL);
			}
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			car[0].x1 += 10;
			car[0].x2 += 10;
			if (car[0].x2 == 900) {
				SetTimer(hWnd, 2, 10 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 3, 30 + speed, NULL);
				check[0] += 1;
			}
			if (car[0].x1 == 900) {
				car[0].x1 = -150;
				car[0].x2 = 0;
				hi++;
				KillTimer(hWnd, 1);
			}

			if (car[0].x2 == 300) {
				KillTimer(hWnd, 1);
				stop++;
			}
			break;

		case 2:
			car[1].x1 += 10;
			car[1].x2 += 10;
			if (car[1].x2 == 900) {
				SetTimer(hWnd, 1, 10 + speed, NULL);
				if(RedTraffic == 0)
					SetTimer(hWnd, 4, 30 + speed, NULL);
				check[0] += 1;
			}
			if (car[1].x1 == 900) {
				car[1].x1 = -150;
				car[1].x2 = 0;
				hi++;
				KillTimer(hWnd, 2);
			}
			if (car[1].x2 == 300) {
				KillTimer(hWnd, 2);
				stop++;
			}
			break;
		case 3:
			car[2].x1 -= 10;
			car[2].x2 -= 10;
			if (car[2].x1 == 0) {
				SetTimer(hWnd, 4, 30 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 5, 50 + speed, NULL);
				check[1] += 1;
			}
			if (car[2].x2 == 0) {
				car[2].x1 = 900;
				car[2].x2 = 1050;
				hi++;
				KillTimer(hWnd, 3);
			}
			if (car[2].x1 == 600) {
				KillTimer(hWnd, 3);
				stop++;
			}
			break;

		case 4:
			car[3].x1 -= 10;
			car[3].x2 -= 10;
			if (car[3].x1 == 0) {
				SetTimer(hWnd, 3, 30 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 6, 50 + speed, NULL);
				check[1] += 1;
			}
			if (car[3].x2 == 0) {
				car[3].x1 = 900;
				car[3].x2 = 1050;
				hi++;
				KillTimer(hWnd, 4);
			}
			if (car[3].x1 == 600) {
				KillTimer(hWnd, 4);
				stop++;
			}
			break;

		case 5:
			car[4].y1 += 10;
			car[4].y2 += 10;
			if (car[4].y1 == 900) {
				SetTimer(hWnd, 6, 50 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 7, 70 + speed, NULL);
				check[2] += 1;
			}
			if (car[4].y2 == 900) {
				KillTimer(hWnd, 5);
				hi++;
				car[4].y1 = 0;
				car[4].y2 = -150;
			}
			if (car[4].y1 == 300) {
				KillTimer(hWnd, 5);
				stop++;
			}
			break;

		case 6:
			car[5].y1 += 10;
			car[5].y2 += 10;
			if (car[5].y1 == 900) {
				SetTimer(hWnd, 5, 50 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 8, 70 + speed, NULL);
				check[2] += 1;
			}
			if (car[5].y2 == 900) {
				KillTimer(hWnd, 6);
				hi++;
				car[5].y1 = 0;
				car[5].y2 = -150;
			}
			if (car[5].y1 == 300) {
				KillTimer(hWnd, 6);
				stop++;
			}
			break;

		case 7:
			car[6].y1 -= 10;
			car[6].y2 -= 10;
			if (car[6].y1 == 0) {
				SetTimer(hWnd, 8, 70 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 2, 10 + speed, NULL);
				check[3] += 1;
			}
			if (car[6].y2 == 0) {
				KillTimer(hWnd, 7);
				hi++;
				car[6].y1 = 900;
				car[6].y2 = 1050;
			}
			if (car[6].y1 == 600) {
				KillTimer(hWnd, 7);
				stop++;
			}
			break;

		case 8:
			car[7].y1 -= 10;
			car[7].y2 -= 10;
			if (car[7].y1 == 0) {
				SetTimer(hWnd, 7, 70 + speed, NULL);
				if (RedTraffic == 0)
					SetTimer(hWnd, 1, 10 + speed, NULL);
				check[3] += 1;
			}
			if (car[7].y2 == 0) {
				KillTimer(hWnd, 8);
				hi++;
				car[7].y1 = 900;
				car[7].y2 = 1050;
			}
			if (car[7].y1 == 600) {
				KillTimer(hWnd, 8);
				stop++;
			}
			break;

		case 9:
			if (YellowC == 0)
				YellowC = 1;
			else if (YellowC == 1) {
				KillTimer(hWnd, 9);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				KillTimer(hWnd, 5);
				KillTimer(hWnd, 6);
				KillTimer(hWnd, 7);
				KillTimer(hWnd, 8);
				YellowC = 0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CHAR:
		if (wParam == '+')
			speed -= 5;
		else if (wParam == '-')
			speed += 5;
		break;
	case WM_PAINT: //�޽����� ó��
		hdc = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, 900, 900);
		hbmMemOld = (HBITMAP)SelectObject(memDC, hBitmap);
		PatBlt(memDC, 0, 0, 900, 900, WHITENESS);
		SelectObject(memDC, (HBITMAP)hBitmap);

		Rectangle(memDC, 610, 10, 830, 110);
		if (RedTraffic == 1) { // ���� ��ȣ���϶� üũ
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
			Ellipse(memDC, 620, 30, 680, 90);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		}
		else if(RedTraffic == 0)
			Ellipse(memDC, 620, 30, 680, 90);
		
		if (YellowC == 1) {
			RedTraffic = 1;
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
			Ellipse(memDC, 690, 30, 750, 90);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		}
		else if(YellowC == 0)
			Ellipse(memDC, 690, 30, 750, 90);


		if (RedTraffic == 0) { // �ʷ� ��ȣ���϶� üũ
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
			Ellipse(memDC, 760, 30, 820, 90);
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		}
		else if(RedTraffic == 1)
			Ellipse(memDC, 760, 30, 820, 90);



		for (int i = 1; i < 3; i++) {
			MoveToEx(memDC, 300 * i, 0, NULL);
			LineTo(memDC, 300 * i, 900);
		}
		for (int i = 1; i < 3; i++) {
			MoveToEx(memDC, 0, 300 * i, NULL);
			LineTo(memDC, 900, 300 * i);
		}
		MoveToEx(memDC, 450, 0, NULL);
		LineTo(memDC, 450, 300);
		MoveToEx(memDC, 450, 600, NULL);
		LineTo(memDC, 450, 900);
		MoveToEx(memDC, 0, 450, NULL);
		LineTo(memDC, 300, 450);
		MoveToEx(memDC, 600, 450, NULL);
		LineTo(memDC, 900, 450);
		
		for (int i = 0; i < 8; ++i)
			Rectangle(memDC, car[i].x1, car[i].y1, car[i].x2, car[i].y2);


		if (stop == 5)
			SetTimer(hWnd, 1, 10, NULL);

		BitBlt(hdc, 0, 0, 900, 900, memDC, 0, 0, SRCCOPY);

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
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}