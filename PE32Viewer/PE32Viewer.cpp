// PE32Viewer.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "PE32Viewer.h"

#include <sstream>
#include <iostream>

// Точка входа программы
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MainWindow win;

    if (!win.Create("PE32", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, 0, 1280, 960))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);


    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}