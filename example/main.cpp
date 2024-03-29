// Example Implementation

#define OEMRESOURCE  // To get access to things like OCR_NORMAL

#include <cassert>
#include <Windows.h>
#include "NotifcationIcon.h"

// Copied from the SDK example, not used in this example
constexpr UINT WMAPP_NOTIFYCALLBACK = WM_APP + 1;
constexpr GUID iconGUID =
    {0x02654e2b, 0x8884, 0x4aa0, {0x86, 0x93, 0xf9, 0x66, 0xd1, 0x40, 0xe9, 0xcd}};

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
    while( (bRet = GetMessage(&msg, hWnd, 0, 0)) )
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
        "Notification Icon",
        WS_SYSMENU|WS_VISIBLE,
        100,
        100,
        500,
        500,
        NULL,
        NULL,
        NULL,
        NULL
    );
    assert(win);

    HWND txt = CreateWindow(
        "STATIC",
        "Example Window",
        WS_CHILD|WS_VISIBLE,
        5,
        5,
        483,
        460,
        win,
        NULL,
        NULL,
        NULL
    );
    assert(txt);

    NotifcationIcon icon(iconGUID);
    icon.setup(win, ico, "Test Icon", WMAPP_NOTIFYCALLBACK);
    icon.showNotification("So, this is working...", "Again");

    int exitCode = WindowMessageLoop(win);
    icon.remove();
    return exitCode;
}