#include "CodeWindow.h"
#include "MainWindow.h"
#include "PE32ViewerConstants.h"

PCSTR  CodeWindow::ClassName() const { return "CODEWINDOW"; }

CodeWindow::CodeWindow(MainWindow* ParentWindow, string sTitle, LONG lBegOfCode, LONG lEndOfCode) :
    ParentWindow(ParentWindow), sTitle(sTitle) {
    this->lBegOfCode = lBegOfCode;
    this->lEndOfCode = lEndOfCode;
};

LRESULT CodeWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        std::ios::sync_with_stdio(0);
        dwCountOfPages = DWORD(ceil(((lEndOfCode - lBegOfCode + 1)) / (16 * nRowsOnPage))) + 1;
        hForwardButton = CreateWindowA("button", S_FORWARD, WS_VISIBLE | WS_CHILD | ES_CENTER, 1065, 50, 100, 30, hWnd, (HMENU)IDC_FORWARDBUTTON, GetModuleHandle(NULL), NULL);
        hBackButton = CreateWindowA("button", S_BACK, WS_VISIBLE | WS_CHILD | ES_CENTER, 1065, 120, 100, 30, hWnd, (HMENU)IDC_BACKBUTTON, GetModuleHandle(NULL), NULL);
        hPageSwitcher = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 1065, 180, 40, 30, hWnd, NULL, GetModuleHandle(NULL), NULL);
        hPageSwitcherButton = CreateWindowA("button", S_SWITCH, WS_VISIBLE | WS_CHILD | ES_CENTER, 1110, 180, 60, 30, hWnd, (HMENU)IDC_SWITCHBUTTON, GetModuleHandle(NULL), NULL);
        OpenPage(0);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDC_FORWARDBUTTON:
        {
            dwPage++;
            OpenPage(dwPage);
            break;
        }
        case IDC_BACKBUTTON:
        {
            dwPage--;
            OpenPage(dwPage);
            break;
        }
        case IDC_SWITCHBUTTON:
        {
            char sPage[10];
            GetWindowText(hPageSwitcher, sPage, 10);
            DWORD dwNewPage;
            try {
                dwNewPage = std::stoul(sPage) - 1;
            }
            catch (...) {
                break;
            }
            if (dwNewPage < dwCountOfPages) {
                dwPage = dwNewPage;
                OpenPage(dwPage);
            }
            break;
        }
        }
    }
    case WM_DESTROY:
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
    }
    return 0;

    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

BOOL CodeWindow::ShowCodeWindow() {

    if (hWnd) {
        DestroyWindow(hWnd);
    }
    Create(sTitle.c_str(), WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, 0, 1200, 520, nullptr, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, SW_NORMAL);
    UpdateWindow(hWnd);

    return TRUE;
}

HANDLE CodeWindow::hCreateHandleOfPEFile() {
    HANDLE hf;
    hf = CreateFile(ParentWindow->szFile,
        GENERIC_READ,
        0,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    return hf;
}

BYTE CodeWindow::GetBYTEFromPEFile(HANDLE hPEFile) {
    BYTE BData;
    BOOL bIsCorrect = ReadFile(hPEFile, &BData, 1, NULL, NULL);
    if (bIsCorrect) {
        return BData;
    }
    else {
        return 0;
    }
}

string CodeWindow::GetHexStringFromData(LONG lData) const {
    std::stringstream ssStream;
    ssStream << "0x" << std::setfill('0') << std::setw(8) << std::hex << std::uppercase << lData;
    return ssStream.str();
}

string CodeWindow::GetHexStringFromData(BYTE BData) const {
    std::stringstream ssStream;
    ssStream << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << +BData;
    return ssStream.str();
}

inline string CodeWindow::GetUTF8StringFromBYTE(BYTE BData) {
    return string(1, BData);
}

VOID CodeWindow::AddItemToTable(string sItem, int nLine, int nColumn)
{
    LVITEM lvi = { 0 };
    lvi.mask = LVIF_TEXT;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nLine;
    lvi.pszText = const_cast<char*>(sItem.c_str());
    if (nColumn == 0) {
        ListView_InsertItem(hCodeTable, &lvi);
    }
    else {
        lvi.iSubItem = nColumn;
        ListView_SetItem(hCodeTable, &lvi);
    }
    return;
}


BOOL CodeWindow::OpenPage(LONG dwPage) {
    if (hCodeTable) {
        DestroyWindow(hCodeTable);
    }
    LONG lLocalBegOfCode = nRowsOnPage * dwPage * 16 + lBegOfCode;
    BYTE nTailOfBytes = lLocalBegOfCode % 16;
    BYTE nModTailOfBytes = nTailOfBytes == 0 ? 0 : 16 - nTailOfBytes;
    BOOL bBegOfCode = (!dwPage);
    if (!bBegOfCode) {
        lLocalBegOfCode += nModTailOfBytes;
    }
    BOOL bEndOfCode = ((lLocalBegOfCode + nRowsOnPage * 16) >= lEndOfCode);
    hCountOfPages = CreateWindowEx(0, "STATIC", (std::to_string(dwPage + 1) + " / " + std::to_string(dwCountOfPages)).c_str(), WS_CHILD | WS_VISIBLE, 1065, 90, 100, 15, hWnd, 0, GetModuleHandle(NULL), NULL);
    if (bBegOfCode) {
        EnableWindow(hBackButton, FALSE);
    }
    else {
        EnableWindow(hBackButton, TRUE);
    }
    if (bEndOfCode) {
        EnableWindow(hForwardButton, FALSE);
    }
    else {
        EnableWindow(hForwardButton, TRUE);
    }
    if (!bBegOfCode) {
        nModTailOfBytes = 0;;
    }
    LONG lLocalEndOfCode = bEndOfCode ? lEndOfCode : lLocalBegOfCode + nRowsOnPage * 16 - 1 + nModTailOfBytes;

    hCodeTable = CreateWindowA(
        WC_LISTVIEW,
        "",
        WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_BORDER,
        0, 0,
        1057,
        480,
        hWnd,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    LVCOLUMN lvc;
    BYTE iCol;

    lvc.mask = LVCF_FMT | LVCFMT_CENTER | LVCF_TEXT;
    lvc.iSubItem = 0;
    lvc.pszText = nullptr;
    lvc.cx = 80;
    lvc.fmt = LVCFMT_CENTER;
    if (ListView_InsertColumn(hCodeTable, 0, &lvc) == -1)
        return FALSE;
    lvc.cx = 30;
    lvc.mask += LVCF_SUBITEM;
    for (BYTE i = 0; i < 2; i++) {
        for (iCol = 1; iCol <= 16; iCol++)
        {
            char sHex[2] = { (iCol - 1 < 10) ? '0' + iCol - 1 : 'a' - 10 + iCol - 1, '\0' };
            lvc.iSubItem = i * 17 + iCol;
            lvc.pszText = sHex;

            if (ListView_InsertColumn(hCodeTable, i * 17 + iCol, &lvc) == -1)
                return FALSE;
        }
        if (i == 0) {
            lvc.iSubItem = 17;
            lvc.pszText = nullptr;
            if (ListView_InsertColumn(hCodeTable, 17, &lvc) == -1)
                return FALSE;
            lvc.cx = 28;
        }
    }
    if (bBegOfCode) {
        if (nTailOfBytes != 0) {
            AddItemToTable(GetHexStringFromData(lLocalBegOfCode - nTailOfBytes), 0, 0);
            for (BYTE i = 1; i < nTailOfBytes; i++) {
                AddItemToTable("", 1, i);
                AddItemToTable("", 1, 17 + i);
            }
        }
    }
    else {
        nTailOfBytes = 0;
    }

    LONG nByte = lLocalBegOfCode;
    BYTE nColumn = 1 + nTailOfBytes;
    LONG lRow = 0;
    HANDLE hPEFile = hCreateHandleOfPEFile();
    SetFilePointer(hPEFile, lLocalBegOfCode, NULL, 0);
    while (nByte <= lLocalEndOfCode) {
        if (nColumn == 1) {
            AddItemToTable(GetHexStringFromData(nByte), lRow, 0);
        }
        BYTE BBYTEFromPEFile = GetBYTEFromPEFile(hPEFile);
        AddItemToTable(GetHexStringFromData(BBYTEFromPEFile), lRow, nColumn);
        AddItemToTable(GetUTF8StringFromBYTE(BBYTEFromPEFile), lRow, 17 + nColumn);
        nColumn += 1;
        if (nColumn == 17) {
            nColumn = 1;
            lRow++;
        }
        nByte++;
    }
    CloseHandle(hPEFile);
    return TRUE;
}

CodeWindow::~CodeWindow() {
    DestroyWindow(hWnd);
}