#include <windows.h>
#include <stdlib.h>
#include <commdlg.h>
#include <cstdio>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4 
#define OPEN_FILE_BUTTON 5
#define SAVE_FILE_BUTTON 6


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void displayDialog(HWND);

HMENU hMenu;
HWND hName, hSaveFile, hAge, hOut, hMainWindow, hOpenFile, hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	//MessageBoxW(NULL, L"HELLO", L"HI", MB_OK);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
void display_file(char* path)
{
	FILE  *file;
	file = fopen(path, "r");
	fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char *data = new char[_size + 1];
	fread(data, _size, 1, file);
	data[_size] = '\0';

	SetWindowText(hEdit, (LPCWSTR)data);

	fclose(file);
}

void open_file(HWND hWnd)
{
	OPENFILENAMEA ofn;

	char file_name[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All file\0*.*\0Source File\0*.CPP\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;

	GetOpenFileNameA(&ofn);
	display_file(ofn.lpstrFile);
}

void write_file(char* path)
{
	FILE *file;
	file = fopen(path, "w");
	
	int _size = GetWindowTextLengthA(hEdit);
	char *data = new char[_size + 1];
	GetWindowTextA(hEdit, data, _size+1);

	fwrite(data, _size + 1, 1, file);

	fclose(file);
}

void save_file(HWND hWnd)
{
	OPENFILENAMEA ofn;

	char file_name[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All file\0*.*\0Source File\0*.CPP\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;

	GetSaveFileNameA(&ofn);

	write_file((char*)ofn.lpstrFile);

	//display_file(ofn.lpstrFile);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int val;
	switch (msg)
	{
	case WM_COMMAND:
		switch(wp)
		{
		case OPEN_FILE_BUTTON:
			open_file(hWnd);
			break;
		case SAVE_FILE_BUTTON:
			save_file(hWnd);
			break;
		}
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenuA(hSubMenu, MF_STRING, NULL, "SubMenuItem");

	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenuA(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
	AppendMenuA(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	//Append Menu in Windows default to AppendMenuW so needed to change to AppendMenuA
	AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenuA(hMenu, MF_STRING, NULL, "Help");
	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
	hOpenFile = CreateWindowW(L"Button", L"Open File", WS_VISIBLE | WS_CHILD |WS_BORDER, 0, 0, 98, 38, hWnd, (HMENU)OPEN_FILE_BUTTON, NULL, NULL);
	hSaveFile = CreateWindowW(L"Button", L"Save File", WS_VISIBLE | WS_CHILD | WS_BORDER, 170, 0, 98, 38, hWnd, (HMENU)SAVE_FILE_BUTTON, NULL, NULL);
	hEdit = CreateWindowW(L"Edit", L" ", WS_VISIBLE | WS_CHILD |ES_MULTILINE | WS_VSCROLL| WS_HSCROLL, 0, 40, 450, 390, hWnd, NULL, NULL, NULL);
}

