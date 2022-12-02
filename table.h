#define DEFAULT_TABLE_VALUE "NULL"
#define ID_LISTVIEW 1

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE (-6)
#endif

char **parceTablesData;
HWND hwndList;
LVITEM lvi;

int CreateItems(HWND hwndList, wchar_t *Text1, wchar_t *Text2, wchar_t *Text3, wchar_t *Text4)
{ 
    lvi.iSubItem = 0;
    lvi.iItem = 0;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iSubItem = 0;

    ListView_InsertItem(hwndList, &lvi);

    ListView_SetItemText(hwndList, 0, 0, Text1);
    ListView_SetItemText(hwndList, 0, 1, Text2);
    ListView_SetItemText(hwndList, 0, 2, Text3);
    ListView_SetItemText(hwndList, 0, 3, Text4);

    return 0;
}

int CreateColumn(HWND hwndLV, int iCol, wchar_t * text, int iWidth)
{
    LVCOLUMN lvc;

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = iWidth;
    lvc.pszText = text;
    lvc.iSubItem = iCol;

    return ListView_InsertColumn(hwndLV, iCol, & lvc);
}

void clearTable()
{
    ListView_DeleteAllItems(hwndList);
}

void insertData()
{
    clearTable();

    char *tablesData = readFile();

    parceTablesData = parceString(tablesData, FILE_BASE_DELIMITER);

    long linesTable = countLines();

    for (int i = linesTable - 1; i >= 0; i--)
    {
        char **arrayTableElements = parceString(copyString(parceTablesData[i]), FILE_STRING_DELIMITER);
        CreateItems(hwndList, 
            GetWC(arrayTableElements[0]), GetWC(arrayTableElements[1]), GetWC(arrayTableElements[2]), GetWC(arrayTableElements[3]));   
    }
}

void CreateMenuItem(HWND hwnd)
{
    initFile();
    
    InitCommonControls();
    
    hwndList = CreateWindow(WC_LISTVIEW, NULL, 
        WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS, 
        0, 0, 640, 400, 
        hwnd, ID_LISTVIEW, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

    ListView_SetExtendedListViewStyle(hwndList,
        LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES | LVS_OWNERDATA | LVS_SORTASCENDING);

    CreateColumn(hwndList, 1, L"Id", 40);
    CreateColumn(hwndList, 2, L"Title", 200);
    CreateColumn(hwndList, 3, L"Description", 250);
    CreateColumn(hwndList, 3, L"Link", 150);

    insertData();
}
