#define FILE_MENU_CREATE    1
#define FILE_MENU_EDIT      2
#define FILE_MENU_DELETE    3
#define FILE_MENU_HELP      4

void AddHeadersMenu(HWND hwnd) 
{
    HMENU hmenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP | MFT_RIGHTJUSTIFY, (UINT_PTR)hFileMenu, L"File");

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_CREATE, L"Add");
    AppendMenu(hFileMenu, MF_STRING, 2, L"Edit");
    AppendMenu(hFileMenu, MF_STRING, 3, L"Delete");
    AppendMenu(hFileMenu, MFT_SEPARATOR, NULL, NULL);
    AppendMenu(hFileMenu, MF_STRING, 4, L"Help");

    SetMenu(hwnd, hmenu);
}
