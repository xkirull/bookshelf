#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "commctrl.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

HWND hMainWindow;

#include "file.h"
#include "utils.h"
#include "menu.h"
#include "table.h"
#include "dialog.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    char *locale = setlocale(LC_ALL, "");

    const wchar_t CLASS_NAME[]  = L"Window Class";
    
    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    RegisterDialogClass(hInstance);

    hMainWindow = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Program Windows",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX,    

        CW_USEDEFAULT, CW_USEDEFAULT, 645, 450,

        NULL,       
        NULL,       
        hInstance,  
        NULL        
    );

    if (hMainWindow == NULL)
    {
        return 0;
    }

    ShowWindow(hMainWindow, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_NOTIFY:
            {
                switch (((LPNMHDR)lParam)->code)
                {
                    case NM_DBLCLK:
                    Cleanup: ;
                        int iSlected = SendMessage(hwndList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

                        if(parceTablesData[iSlected]) {
                            char *actionLink = parceString(copyString(parceTablesData[iSlected]), FILE_STRING_DELIMITER)[3];
                            
                            if(strstr(actionLink, "http://") != NULL || strstr(actionLink, "https://") != NULL){
                                openLink(actionLink);
                            } else {
                                MessageBeep(MB_ICONERROR);
                                MessageBox(hwnd, L"Error! The link provided is invalid.", L"Linked error", MB_ICONERROR);
                            }
                        }

                        break; 
                }
            }

            return 0;

        case WM_COMMAND:

            switch (wParam) {
                case FILE_MENU_CREATE:
                    DisplayDialog(hwnd, DIALOG_CREATE_TYPE);
                    MessageBeep(MB_ICONINFORMATION);
                    break;

                case FILE_MENU_EDIT:
                FileEdit: ;
                    int iSlected = SendMessage(hwndList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

                    if(iSlected < 0) {
                        MessageBox(hwnd, L"You haven't chosen anything!", L"Nothing selected", MB_ICONINFORMATION);
                        break;
                    }

                    if(parceTablesData[iSlected]) {
                        DisplayArgs displayArgs = {};

                        char **arrayElements = parceString(copyString(parceTablesData[iSlected]), FILE_STRING_DELIMITER);

                        displayArgs.title = arrayElements[1];
                        displayArgs.description = arrayElements[2];
                        displayArgs.link = arrayElements[3];

                        DisplayDialog(hwnd, DIALOG_EDIT_TYPE);    
                        setDefaultParams(displayArgs);
                        MessageBeep(MB_ICONINFORMATION);
                    }

                    break;

                case FILE_MENU_DELETE:
                FileDelete: ;
                    int deleteIndex = SendMessage(hwndList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

                    if(deleteIndex < 0) {
                        MessageBox(hwnd, L"You haven't chosen anything!", L"Nothing selected", MB_ICONINFORMATION);
                        break;
                    }

                    if(parceTablesData[deleteIndex]) {
                        deleteItemFromArray(parceTablesData, deleteIndex);
                        updateItemsIndex(parceTablesData, countLines() - 1);
                        writeFile(parceTablesData, countLines() - 1);
                        insertData();

                        MessageBeep(MB_OK);                        
                    }

                    break;

                case FILE_MENU_HELP:
                    openLink("https://github.com/xkirull/bookshelf");
                    break;
            }

            return 0;

        case WM_CREATE:
            AddHeadersMenu(hwnd);
            CreateMenuItem(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

                EndPaint(hwnd, &ps);
            }
            return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
