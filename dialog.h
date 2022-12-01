#define DIALOG_CREATE_TYPE 1
#define DIALOG_EDIT_TYPE 2

#define CREATE_BUTTON 11
#define EDIT_BUTTON 12

HWND hTitle, hDescription, hLink, hButton;

typedef struct {
    char *title;
    char *description;
    char *link;
} DisplayArgs;

typedef struct {
    char *value;
    size_t size;
} DisplayOutData;

DisplayOutData addTableStringToData(unsigned int index)
{
    char title[255], description[255], link[255];

    GetWindowTextA(hTitle, title, 255);
    GetWindowTextA(hDescription, description, 255);
    GetWindowTextA(hLink, link, 255);

    strcpy(title, stringFull(title, DEFAULT_TABLE_VALUE));
    strcpy(description, stringFull(description, DEFAULT_TABLE_VALUE));
    strcpy(link, stringFull(link, DEFAULT_TABLE_VALUE));

    DisplayOutData outData = {};

    outData.size = sizeof(title) + sizeof(description) + sizeof(link);

    outData.value = malloc(outData.size);

    snprintf(outData.value, outData.size, "%d | %s | %s | %s", index, title, description, link);

    return outData;
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            switch(wParam)
            {
                case CREATE_BUTTON:
                CreateButton: ;
                    DisplayOutData CreateOutData = addTableStringToData(0);

                    appendArray(parceTablesData, CreateOutData.value, CreateOutData.size);

                    updateItemsIndex(parceTablesData, countLines() + 1);

                    writeFile(parceTablesData, countLines() + 1);

                    insertData();

                    EnableWindow(hMainWindow, TRUE);
                    DestroyWindow(hwnd);

                    break;
                
                case EDIT_BUTTON:
                EditButton: ;
                    size_t indexValue = SendMessage(hwndList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
                    DisplayOutData EditOutData = addTableStringToData(indexValue + 1);

                    setValueInArray(parceTablesData, EditOutData.value, EditOutData.size, indexValue);

                    writeFile(parceTablesData, countLines());

                    insertData();

                    EnableWindow(hMainWindow, TRUE);
                    DestroyWindow(hwnd);
                    
                    break;
            }

            return 0;

        case WM_CLOSE:
            EnableWindow(hMainWindow, TRUE);
            DestroyWindow(hwnd);
            break;
        
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

void RegisterDialogClass(HINSTANCE hInstance)
{    
    WNDCLASSW dialog = {};

    dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
    dialog.hCursor       = LoadCursor(NULL, IDC_ARROW);
    dialog.hInstance     = hInstance;
    dialog.lpszClassName = L"DialogClass";
    dialog.lpfnWndProc   = DialogProc;

    RegisterClassW(&dialog);
}

void CreateControls(HWND hwnd, int buttonType)
{
    CreateWindowW(L"Static", L"Title: ", WS_VISIBLE | WS_CHILD, 70, 25, 100, 38, hwnd, NULL, NULL, NULL);
    hTitle = CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 110, 23, 160, 20, hwnd, NULL, NULL, NULL);

    CreateWindowW(L"Static", L"Description: ", WS_VISIBLE | WS_CHILD, 24, 55, 100, 38, hwnd, NULL, NULL, NULL);
    hDescription = CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 110, 53, 160, 20, hwnd, NULL, NULL, NULL);

    CreateWindowW(L"Static", L"Link: ", WS_VISIBLE | WS_CHILD, 71, 85, 100, 38, hwnd, NULL, NULL, NULL);
    hLink = CreateWindowW(L"Edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 110, 83, 160, 20, hwnd, NULL, NULL, NULL);

    if(buttonType == DIALOG_CREATE_TYPE) {
        hButton = CreateWindowW(L"Button", L"Create", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_CLIPSIBLINGS | WS_TABSTOP, 125, 125, 80, 30, hwnd, (HMENU)CREATE_BUTTON, NULL, NULL);
    }

    if(buttonType == DIALOG_EDIT_TYPE) {
        hButton = CreateWindowW(L"Button", L"Edit", 
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_CLIPSIBLINGS | WS_TABSTOP, 125, 125, 80, 30, hwnd, (HMENU)EDIT_BUTTON, NULL, NULL);
    }
}

void setDefaultParams(DisplayArgs displayArgs)
{
    SetWindowTextW(hTitle, GetWC(displayArgs.title));
    SetWindowTextW(hDescription, GetWC(displayArgs.description));
    SetWindowTextW(hLink, GetWC(displayArgs.link));
}

void DisplayDialog(HWND hwnd, int buttonType)
{
    HWND dialogHwnd = CreateWindowW(L"DialogClass", L"Dialog", 
        WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX | SC_CLOSE, CW_USEDEFAULT, CW_USEDEFAULT, 330, 200, hwnd, NULL, NULL, NULL);

    EnableWindow(hwnd, FALSE);

    CreateControls(dialogHwnd, buttonType);
}
