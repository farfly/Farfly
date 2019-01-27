#include <windows.h>

typedef struct {
    DWORD   cbSize;
    HWND    hwnd;
    LPSTR   lpszClassName;
    HWND    hwndParent;
}STRU_WND_INFO, *PSTRU_WND_INFO;

static BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)
{
    HWND* pphWnd = (HWND*)lParam;

    if (GetParent(hwnd))
    {
        *pphWnd = NULL;
        return TRUE;
    }
    *pphWnd = hwnd;
    return FALSE;
}

static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    PSTRU_WND_INFO pstruWndInfo = (PSTRU_WND_INFO)lParam;
    RECT rect;
    CHAR lpszClassName[100];
    GetClassName(hwnd, lpszClassName, 100);
    if ( strcmp(lpszClassName, pstruWndInfo->lpszClassName) == 0 )
    {
        pstruWndInfo->hwnd = hwnd;
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_EX_CLIENTEDGE);
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_EX_WINDOWEDGE);
        memset(&rect, 0, sizeof(rect));
        GetWindowRect(pstruWndInfo->hwndParent, &rect);
        SetWindowPos(hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

    }
    return TRUE;
}

LONG _declspec(dllexport) ToggleFullScreen(LPSTR lpszChildWndClassName)
{
    HWND hTop = NULL;
    DWORD dwThreadID;
    STRU_WND_INFO struWndInfo;
    int cx = 0;
    int cy = 0;

    dwThreadID = GetCurrentThreadId();
    EnumThreadWindows(dwThreadID, FindWindowProc, (LPARAM)&hTop);

    if (hTop)
    {
        /* Determine the current state of the window */
        if ( GetWindowLong(hTop, GWL_STYLE) & WS_CAPTION )
        {
            HMONITOR hMonitor = MonitorFromWindow(hTop, MONITOR_DEFAULTTONEAREST);
            MONITORINFO pmi;
            memset(&pmi, 0, sizeof(pmi));
            pmi.cbSize = sizeof(pmi);
            if (GetMonitorInfo(hMonitor, &pmi))
            {
                /* Remove border, caption, and edges */
                SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_EXSTYLE) & ~WS_BORDER);
                SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) & ~WS_CAPTION);
                SetWindowLong(hTop, GWL_EXSTYLE, GetWindowLong(hTop, GWL_STYLE) & ~WS_EX_CLIENTEDGE);
                SetWindowLong(hTop, GWL_EXSTYLE, GetWindowLong(hTop, GWL_STYLE) & ~WS_EX_WINDOWEDGE);

                cx = pmi.rcMonitor.right - pmi.rcMonitor.left;
                cy = pmi.rcMonitor.bottom - pmi.rcMonitor.top;
                cx < 0 ? cx = 0 - cx : cx;
                SetWindowPos(hTop, HWND_TOP, pmi.rcMonitor.left, pmi.rcMonitor.top,
                             cx, cy, SWP_SHOWWINDOW);

                /* Now need to find the child text area window
                 * and set it's size accordingly
                 */
                memset(&struWndInfo, 0, sizeof(struWndInfo));
                struWndInfo.cbSize        = sizeof(struWndInfo);
                struWndInfo.lpszClassName = lpszChildWndClassName;
                struWndInfo.hwndParent    = hTop;
                EnumChildWindows(hTop, EnumChildProc, (LPARAM)&struWndInfo);
            }
        }
        else
        {
            /* Already full screen, so restore all the previous styles */
            SetWindowLong(hTop, GWL_EXSTYLE, GetWindowLong(hTop, GWL_EXSTYLE) | WS_BORDER);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_CAPTION);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_SYSMENU);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_MINIMIZEBOX);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_MAXIMIZEBOX);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_SYSMENU);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_EX_CLIENTEDGE);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_EX_WINDOWEDGE);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_THICKFRAME);
            SetWindowLong(hTop, GWL_STYLE, GetWindowLong(hTop, GWL_STYLE) | WS_DLGFRAME);

            SendMessage(hTop, WM_SYSCOMMAND, SC_RESTORE, 0);
            SendMessage(hTop, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        }
    }
    return GetLastError();
}

