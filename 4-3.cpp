//
#include <windows.h>
#include <TCHAR.h>

// 구현된것
// p, m, s, 엔터!, q, 선택하기등등

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WindowWidth = 1280;
int WindowHeight = 1000;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASSEX	WndClass;
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hwnd = CreateWindow(_T("Window Class Name"), _T("2018 Window Programming Midterm"), WS_OVERLAPPEDWINDOW, 0, 0, 1280, 1000, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

struct Shot {
	int x;
	int y;
	int R;
	int G;
	int B;
	int downSpeed;
	int crash = 0;
};

struct box {
	int x;
	int y;
	int R;
	int G;
	int B;
	int boxP = 20;
	int movebox;
};

struct boxGun {
	int x;
	int y;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hpen, oldpen;
	HBRUSH hbrush, oldBrush;
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hbitmap;
	static Shot Shot[50];
	static box box[3];
	static int boxCount[3] = { 0 };
	static int inputX, inputY;
	static int select;
	static boxGun boxgun[50];
	static int boxguncount = 1;
	switch (iMsg) {
	case WM_CREATE:
		for (int i = 0; i < 50; ++i)
		{
			Shot[i].x = rand() % 1280;
			Shot[i].y = 30;
			Shot[i].R = rand() % 255;
			Shot[i].G = rand() % 255;
			Shot[i].B = rand() % 255;
			Shot[i].downSpeed = rand() % 10 + 1;
		}
		for (int i = 0; i < 3; ++i)
		{
			box[i].x = 10;
			box[i].y = 670 - (100 * i);
			box[i].R = 255;
			box[i].G = 255;
			box[i].B = 255;
			box[i].movebox = rand() % 10 + 1;
		}

		return 0;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			for (int i = 0; i < 50; ++i)
			{
				Shot[i].y += Shot[i].downSpeed;
				for (int j = 0; j < 3; ++j)
				{
					if (Shot[i].x > box[j].x - box[j].boxP && Shot[i].x < box[j].x + box[j].boxP && Shot[i].y > box[j].y - box[j].boxP && Shot[i].y < box[j].y + box[j].boxP)
					{
						box[j].R = Shot[i].R;
						box[j].G = Shot[i].G;
						box[j].B = Shot[i].B;
						Shot[i].x = rand() % 1280;
						Shot[i].y = 30;
						Shot[i].R = rand() % 255;
						Shot[i].G = rand() % 255;
						Shot[i].B = rand() % 255;
						Shot[i].downSpeed = rand() % 10 + 1;
						if (box[j].boxP <= 45)
							box[j].boxP += 5;
					}

					if (Shot[i].y >= 720) {
						Shot[i].x = rand() % 1280;
						Shot[i].y = 30;
						Shot[i].R = rand() % 255;
						Shot[i].G = rand() % 255;
						Shot[i].B = rand() % 255;
						Shot[i].downSpeed = rand() % 10 + 1;
					}
				}
			}
			break;

		case 2:
			for (int i = 0; i < 3; ++i) {
				if (boxCount[i] % 2 == 0) {
					box[i].x += box[i].movebox;
					if (box[i].x + box[i].boxP >= 1270)
						boxCount[i]++;
				}
				else if (boxCount[i] % 2 == 1) {
					box[i].x -= box[i].movebox;
					if (box[i].x - box[i].boxP <= 10)
						boxCount[i]++;
				}
				if (select == 1) {
					for (int i = 0; i < 50; ++i) {
						boxgun[i].x = box[1].x + box[1].boxP;
						boxgun[i].y = box[1].y;
					}
				}
				if (select == 0) {
					for (int i = 0; i < 50; ++i) {
						boxgun[i].x = box[0].x + box[0].boxP;
						boxgun[i].y = box[0].y;
					}
				}
				if (select == 2) {
					for (int i = 0; i < 50; ++i) {
						boxgun[i].x = box[2].x + box[2].boxP;
						boxgun[i].y = box[2].y;
					}
				}
			}
			break;
		case 3:
			boxguncount = 1;
			for (int i = 0; i < boxguncount; ++i) {
				if (boxCount[0] % 2 == 0 || boxCount[1] % 2 == 0 || boxCount[2] % 2 == 0) {
					boxgun[i].x += 5;
					if (boxgun[i].x >= 1280)
						boxgun[i].x = box[i].x;
				}
				else if (boxCount[0] % 2 == 1 || boxCount[1] % 2 == 1 || boxCount[2] % 2 == 1) {
					boxgun[i].x -= 5;
					if (boxgun[i].x < 0)
						boxgun[i].x = box[i].x;
				}
			}
			boxguncount++;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_LBUTTONDOWN:
		inputX = LOWORD(lParam);
		inputY = HIWORD(lParam);
		for (int j = 0; j < 3; ++j) {
			if (inputX > box[j].x - box[j].boxP &&inputX < box[j].x + box[j].boxP && inputY > box[j].y - box[j].boxP && inputY < box[j].y + box[j].boxP) {
				KillTimer(hwnd, 3);
				if (j == 0) {
					select = 0;
				}
				else if (j == 1) {
					select = 1;
				}
				else if (j == 2)
					select = 2;
			}
		}

		for (int i = 0; i < 3; ++i) {
			if (inputX < box[i].x && inputY < box[i].y + box[i].y + box[i].boxP && inputY > box[i].y - box[i].boxP)
				boxCount[i] += 1;
		}

		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_CHAR:
		hdc = GetDC(hwnd);
		if (wParam == 'p' || wParam == 'P') { // 빠르게
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 1, 30, NULL);
			SetTimer(hwnd, 2, 100, NULL);
		}
		else if (wParam == 'f') { // 중간 스타트!
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 1, 60, NULL);
			SetTimer(hwnd, 2, 100, NULL);
		}

		else if (wParam == 'm' || wParam == 'M') { // 중간
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 1, 60, NULL);
			SetTimer(hwnd, 2, 100, NULL);
		}
		else if (wParam == 'w' || wParam == 'W') { // 느-----리게
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			SetTimer(hwnd, 1, 100, NULL);
			SetTimer(hwnd, 2, 100, NULL);
		}
		else if (wParam == VK_RETURN) {
			SetTimer(hwnd, 3, 30, NULL);
		}
		else if (wParam == 'q' || wParam == 'Q')
			PostQuitMessage(0);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memDC = CreateCompatibleDC(hdc);
		hbitmap = CreateCompatibleBitmap(hdc, 1280, 720);
		hbmMemOld = (HBITMAP)SelectObject(memDC, hbitmap);
		PatBlt(memDC, 0, 0, 1280, 720, WHITENESS);
		SelectObject(memDC, (HBITMAP)hbitmap);
		Rectangle(memDC, 0, 0, 1280, 720);

		for (int i = 1; i <= 3; ++i)
		{
			MoveToEx(memDC, 0, 720 - (100 * i), NULL);
			LineTo(memDC, 1280, 720 - (100 * i));
		}

		for (int i = 0; i < 50; ++i) {
			hbrush = CreateSolidBrush(RGB(Shot[i].G, Shot[i].G, Shot[i].B));
			oldBrush = (HBRUSH)SelectObject(memDC, hbrush);
			Ellipse(memDC, Shot[i].x - 10, Shot[i].y - 10, Shot[i].x + 10, Shot[i].y + 10);
			DeleteObject(hbrush);
		}

		for (int i = 0; i < boxguncount; ++i) {
			Ellipse(memDC, boxgun[i].x - 5, boxgun[i].y - 5, boxgun[i].x + 5, boxgun[i].y + 5);
		}

		for (int i = 0; i < 3; ++i) {
			hbrush = CreateSolidBrush(RGB(box[i].G, box[i].G, box[i].B));
			oldBrush = (HBRUSH)SelectObject(memDC, hbrush);
			if (select == 1 && i == 1) {
				hpen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
				oldpen = (HPEN)SelectObject(memDC, hpen);
			}
			else if (select == 2 && i == 2) {
				hpen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
				oldpen = (HPEN)SelectObject(memDC, hpen);
			}
			else if (select == 0 && i == 0) {
				hpen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
				oldpen = (HPEN)SelectObject(memDC, hpen);
			}
			Rectangle(memDC, box[i].x - box[i].boxP, box[i].y + box[i].boxP, box[i].x + box[i].boxP, box[i].y - box[i].boxP);
			DeleteObject(hbrush);
			DeleteObject(hpen);
			hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldpen = (HPEN)SelectObject(memDC, hpen);
		}

		BitBlt(hdc, 0, 0, 1280, 720, memDC, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
