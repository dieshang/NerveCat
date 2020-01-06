#include <windows.h>

#include "DemoGame.h"
HWND		hWnd ;


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;	


CGame		*pGame = NULL;
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)

{
	RECT rectClient;
	static TCHAR szAppName[] = TEXT ("Demo") ;
	MSG			msg ;
	WNDCLASS	wc ;
	wc.style			= CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc		= WndProc ;
	wc.cbClsExtra		= 0 ;
	wc.cbWndExtra		= 0 ;
	wc.hInstance		= hInstance ;
	wc.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wc.hCursor			= LoadCursor (NULL, IDC_ARROW) ;
	wc.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wc.lpszMenuName		= NULL ;
	wc.lpszClassName	= szAppName ;

	if (!RegisterClass (&wc))
	{
		MessageBox (	NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	GetClientRect(hWnd, &rectClient);
	hWnd = CreateWindow(szAppName,	// window class name
		TEXT("Χס��è"),	// window caption
		WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,	// window style
		//WS_POPUP | WS_VISIBLE,
		0,	// initial x position
		0,	// initial y position
		528,	// initial x size
		750,
		NULL,			// parent window handle
		NULL,	        // window menu handle
		hInstance,	    // program instance handle
		NULL) ; 	    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow (hWnd) ;

	
	if (SUCCEEDED(CoInitialize(NULL)))				//�����ʼ��Com����ɹ�
	{
		pGame = new  CDemoGame(hInstance, hWnd, 60);
		pGame->InitGame();
		//if (SUCCEEDED(init()))						//��ʼ��
		{
			BOOL  bMessage;
			PeekMessage(&msg, NULL, 0, 0,PM_NOREMOVE);
			while(msg.message != WM_QUIT)			//������Ϣѭ��
			{
				bMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				if(bMessage)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				//Render();							//��Ⱦ
				pGame->GameFunc();
			} 
		}
		
		if (pGame)
		{
			delete pGame;
		}
		CoUninitialize();							//�ͷ�Com���

	}

	return 0 ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0) ;
		}	
		return 0;
	case WM_LBUTTONUP:												//����WM_KEYDWON
			pGame->HandleMouseUp(wParam,lParam);						//����HandleKeyDown�������������Ϣ
			return 0;
	/*case WM_LBUTTONDOWN:
		pGame->HandleMouseDown(wParam,lParam);
		return 0;*/
	case WM_DESTROY:
		PostQuitMessage(0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
