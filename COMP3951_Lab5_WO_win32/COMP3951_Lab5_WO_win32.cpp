// COMP3951_Lab5_WO_win32.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "COMP3951_Lab5_WO_win32.h"
#include <commdlg.h>
#include <iostream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool isDrawing = false, isMain = true;          // Drawing flag
POINT prevPoint, currentPoint;                  // Mouse positions

COLORREF windowColor    = RGB(255, 255, 255);   // Window color, default white
COLORREF pen1Color      = RGB(0, 0, 0);         // Pen 1 color, default black
COLORREF pen2Color      = RGB(255, 255, 255);   // Pen 2 color, default white
int pen1Style = PS_SOLID, pen1Thick = 2;        // Pen 1 defaults
int pen2Style = PS_SOLID, pen2Thick = 20;       // Pen 2 defaults

HPEN hPen1 = CreatePen(pen1Style, pen1Thick, pen1Color);    // Pen 1 setup
HPEN hPen2 = CreatePen(pen2Style, pen2Thick, pen2Color);    // Pen 2 setup

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COMP3951LAB5WOWIN32, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMP3951LAB5WOWIN32));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMP3951LAB5WOWIN32));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COMP3951LAB5WOWIN32);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/// <summary>
/// Helper method for drawing lines.
/// </summary>
/// <param name="hdc"></param>
void DrawLine(HDC hdc, int currentX, int currentY)
{
    // Update current point
	currentPoint.x = currentX;
	currentPoint.y = currentY;
	// Draw a line from the current position to the previous position
	MoveToEx(hdc, currentPoint.x, currentPoint.y, nullptr);
	LineTo(hdc, prevPoint.x, prevPoint.y);
	// Update previous point
	prevPoint = currentPoint;
}

/// <summary>
/// Colour picker dialog, set the background colour.
/// </summary>
/// <param name="hWnd"></param>
/// <returns></returns>
void PickColor(HWND hWnd, COLORREF* windowColor)
{
    // Display the color picker dialog
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16]; // array of custom colors 

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = *windowColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc) == TRUE)
    {
        *windowColor = cc.rgbResult;
        // Change the background color of the window
        SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(*windowColor));
        InvalidateRect(hWnd, NULL, TRUE);
    }
}

/// <summary>
/// Generic method for changing the colour of the pen.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="pen"></param>
/// <param name="pencolor"></param>
/// <param name="penStyle"></param>
/// <param name="penThick"></param>
void PickPenColor(HWND hWnd, HPEN* pen, COLORREF* pencolor, int penStyle, int penThick)
{
    // Display the color picker dialog
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16]; // array of custom colors 

    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = RGB(255, 255, 255);
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc) == TRUE)
    {
        *pencolor = cc.rgbResult;
        *pen = (CreatePen(penStyle, penThick, *pencolor));
    }
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            // Add change colour menu
            HMENU hMenu = GetMenu(hWnd);
            AppendMenu(hMenu, MF_STRING, IDM_CHANGECOLOR, L"Change Window Color");
            AppendMenu(hMenu, MF_STRING, IDM_CHANGEPEN1COLOR, L"Change Pen1 Color");
            AppendMenu(hMenu, MF_STRING, IDM_CHANGEPEN2COLOR, L"Change Pen2 Color");
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_CHANGECOLOR:
                PickColor(hWnd, &windowColor);
                break;
            case IDM_CHANGEPEN1COLOR:
                PickPenColor(hWnd, &hPen1, &pen1Color, pen1Style, pen1Thick);
                break;
            case IDM_CHANGEPEN2COLOR:
                PickPenColor(hWnd, &hPen2, &pen2Color, pen2Style, pen2Thick);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN: {
        // Set is drawing flag to true and store the starting point
        isDrawing = true;
        isMain = true;
        prevPoint.x = LOWORD(lParam);
        prevPoint.y = HIWORD(lParam);
        break;
    }
    case WM_RBUTTONDOWN:
    {
		// Set is drawing flag to true and store the starting point
		isDrawing = true;
		isMain = false;
		prevPoint.x = LOWORD(lParam);
		prevPoint.y = HIWORD(lParam);
        break;
    }
    case WM_MOUSEMOVE: {
        if (isDrawing)
        {
            HDC hdc = GetDC(hWnd);
            HPEN dPen = (isMain) ? hPen1 : hPen2;

			// Draw line
			SelectObject(hdc, dPen);
            DrawLine(hdc, LOWORD(lParam), HIWORD(lParam));

            // Release resources
            ReleaseDC(hWnd, hdc);
        }
        break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP: {
		isDrawing = false;
        break;
    }
    case WM_DESTROY:
    {
        DeleteObject(hPen1);
		DeleteObject(hPen2);
		PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
