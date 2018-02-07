// Example Implementation
// In a future iteration this could become the test driver

#include <cassert>
#include <Windows.h>
#include "NotifcationIcon.h"

void pause() {
    system("pause");
}

LRESULT CALLBACK winProc(HWND hwnd, UINT type, WPARAM wparam, LPARAM lparam)
{
    switch(type)
    {

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return CallWindowProc(DefWindowProc, hwnd, type, wparam, lparam);
    }
    return 0;
}

void registerClass(const char *className, HICON ico)
{
    WNDCLASSEX cls;
    LPTSTR cursorImageName = MAKEINTRESOURCE(OCR_NORMAL);
    HANDLE cursorImage = LoadImage(0, cursorImageName, IMAGE_CURSOR, 0, 0, LR_SHARED|LR_DEFAULTSIZE);

    memset(&cls, 0, sizeof(cls));
    cls.cbSize = sizeof(cls);
    cls.style = CS_DBLCLKS;
    cls.lpfnWndProc = winProc;
    cls.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    cls.lpszClassName = className;
    cls.hIcon = ico;
    cls.hCursor = (HCURSOR)cursorImage;
    RegisterClassEx(&cls);
}

int WindowMessageLoop(HWND hWnd)
{
    BOOL bRet;
    MSG msg;
    while( (bRet = GetMessage(&msg, hWnd, 0, 0 )) )
    {
        if(bRet==-1)
        {
            MessageBox(hWnd, "Error Retrieving window messages!", "Error", MB_ICONERROR);
            return -1;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

int main(int argc, char *argv[]) {
    HICON ico = ExtractIcon(0, "Shell32.dll", 2);
    registerClass("parentWin", ico);
    HWND win = CreateWindow(
        "parentWin",
        "Notifcation Icon",
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        NULL,
        NULL
    );
    assert(win);

    NotifcationIcon icon;
    icon.setIcon(ico);
    icon.setTooltip("Test Icon");
    icon.setWindow(win);
    icon.add();
    icon.show();
    icon.showBalloon("So, this is working...", "For real");
    WindowMessageLoop(win);
    //pause();
    return 0;
}