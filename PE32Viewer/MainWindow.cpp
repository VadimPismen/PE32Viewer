#include "MainWindow.h"

PCSTR  MainWindow::ClassName() const { return "PEREADER"; }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        std::ios::sync_with_stdio(0);
        CreateReaderMenu();
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_OPENFILE:
            OpenFileWithDialogue();
            FillPEDataTable();
            break;
        case IDC_CLICKCODEBUTTON:
            size_t unCountOfCodeButtons = CodeButtons.size();
            for (size_t i = 0; i < unCountOfCodeButtons; i++) {
                if ((HWND)lParam == CodeButtons[i]) {
                    ShowCodeWindow(i);
                }
            }
            break;
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
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

VOID MainWindow::CreateReaderMenu() {
    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, IDM_OPENFILE, IDS_OPENFILE);
    SetMenu(hWnd, hMenu);
    return;
}

BOOL MainWindow::OpenFileWithDialogue() {
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PE\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    return GetOpenFileName(&ofn);
}

inline BOOL MainWindow::CloseFile(HANDLE hPEFile) {
    return CloseHandle(hPEFile);
}

HANDLE MainWindow::hCreateHandleOfPEFile() {
    HANDLE hf;
    hf = CreateFile(ofn.lpstrFile,
        GENERIC_READ,
        0,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    return hf;
}

template <typename T>
string MainWindow::GetUTF8DataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh) {
    SetFilePointer(hPEFile, lDistanceToMove, lpDistanceToMoveHigh, ReadFromCurrentPose);
    DWORD stSizeOfType = sizeof(T);
    std::vector<char> buf(stSizeOfType);
    BOOL bIsCorrect = ReadFile(hPEFile, &buf[0], stSizeOfType, NULL, NULL);
    if (bIsCorrect) {
        string strData = string(buf.begin(), buf.end());
        return strData;
    }
    else {
        return "";
    }
}

template <typename T>
T MainWindow::GetDataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh) {
    SetFilePointer(hPEFile, lDistanceToMove, lpDistanceToMoveHigh, ReadFromCurrentPose);
    DWORD stSizeOfType = sizeof(T);
    T TData;
    BOOL bIsCorrect = ReadFile(hPEFile, &TData, stSizeOfType, NULL, NULL);
    if (bIsCorrect) {
        return TData;
    }
    else {
        return NULL;
    }
}

string MainWindow::GetSignatureFromString(string sData) const{
    string sDataNew;
    for (const auto& sChar : sData) {
        if (sChar == '\0') {
            sDataNew.append("\\0");
        }
        else {
            sDataNew += sChar;
        }
    }
    return "\"" + sDataNew + "\"";
}

template <typename T>
string MainWindow::GetHexStringFromData(T TData, BOOL bSetFill) const{
    std::stringstream ssStream;
    if (bSetFill) {
        ssStream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << std::uppercase << TData;
    }
    else {
        ssStream << "0x" << std::hex << std::uppercase << TData;
    }
    return ssStream.str();
}

template <typename T>
string MainWindow::GetHexAndDecStringFromData(T TData) const{
    std::stringstream ssStream;
    ssStream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << std::uppercase << TData;
    ssStream << std::dec << " (" << TData << ")";
    return ssStream.str();
}

string MainWindow::GetStringFromTime(const time_t* tTime) const{
    char sTimeDateStamp[26];
    ctime_s(sTimeDateStamp, sizeof(sTimeDateStamp), tTime);
    return sTimeDateStamp;
}

const char* MainWindow::GetszFile() const{ return szFile; }

void MainWindow::FillPEDataTable() {
    HANDLE hPEFile = hCreateHandleOfPEFile();
    if (hPEFile != 0) {
        string sCheckMZ = GetUTF8DataFromPEFile<WORD>(hPEFile);
        if (sCheckMZ == S_MZ) {

            for (const auto& CodeWnd : CodeWindows) {
                CodeWnd.~CodeWindow();
            }
            CodeWindows.clear();

            for (const auto& hButton : CodeButtons) {
                DestroyWindow(hButton);
            }

            CodeButtons.clear();
            DestroyWindow(hPEFileName);
            DestroyWindow(hPEDataTable);
            TreeView_DeleteAllItems(hPEDataTable);
            hPEFileName = CreateWindowEx(0, "STATIC", szFile, WS_CHILD | WS_VISIBLE, 5, 0, int(strlen(szFile))*10, 20, hWnd, 0, GetModuleHandle(NULL), NULL);
            hPEDataTable = CreateWindowEx(0,
                WC_TREEVIEW,
                "Tree View",
                WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
                5,
                20,
                400,
                880,
                hWnd,
                NULL,
                GetModuleHandle(NULL),
                NULL);

            htiDOS = AddItemToPEDataTable(S_DOSHEADER);
            AddItemToPEDataTable(S_DOSE_MAGIC + GetSignatureFromString(sCheckMZ), htiDOS);

            LONG lAddressOfPE = GetDataFromPEFile<LONG>(hPEFile, FALSE, 0x3C);
            string sCheckPE = GetUTF8DataFromPEFile<DWORD>(hPEFile, FALSE, lAddressOfPE);
            BOOL bCheckPE = (sCheckPE == S_PE);
            AddItemToPEDataTable(S_DOSE_LFANEW + GetHexAndDecStringFromData(lAddressOfPE), htiDOS, bCheckPE);
            htiPE = AddItemToPEDataTable(S_PEHEADER, NULL, bCheckPE);
            AddItemToPEDataTable(S_PESIGNATURE + GetSignatureFromString(sCheckPE), htiPE, bCheckPE);

            if (bCheckPE) {
                CreateCodeButton(S_DOSSTUBCODETITLE, 450, 60, 200, 30, 0x3D, lAddressOfPE - 1);

                AddItemToPEDataTable(S_PEFILEHEADERMACHINE + GetHexStringFromData(GetDataFromPEFile<WORD>(hPEFile), FALSE), htiPE);

                WORD wNumberOfSections = GetDataFromPEFile<WORD>(hPEFile);
                AddItemToPEDataTable(S_PEFILEHEADERNUMBEROFSECTIONS + GetHexAndDecStringFromData(wNumberOfSections), htiPE);

                DWORD dwTimeDateStamp = GetDataFromPEFile<DWORD>(hPEFile);
                time_t tTimeDateStamp = time_t(dwTimeDateStamp);
                time_t tTimeDateStampBigEndian = time_t(_byteswap_ulong(dwTimeDateStamp));
                AddItemToPEDataTable(S_PEFILEHEADERTIMEDATESTAMP + GetStringFromTime(&tTimeDateStamp), htiPE);
                AddItemToPEDataTable(S_PEFILEHEADERTIMEDATESTAMPBIGENDIAN + GetStringFromTime(&tTimeDateStampBigEndian), htiPE);

                DWORD dwPointerToSymbolTable = GetDataFromPEFile<DWORD>(hPEFile);
                AddItemToPEDataTable(S_PEFILEHEADERPOINTERTOSYMBOLTABLE + GetHexAndDecStringFromData(dwPointerToSymbolTable), htiPE, dwPointerToSymbolTable == 0);
                DWORD dwNumberOfSymbols = GetDataFromPEFile<DWORD>(hPEFile);
                AddItemToPEDataTable(S_PEFILEHEADERNUMBEROFSYMBOLS + GetHexAndDecStringFromData(dwNumberOfSymbols), htiPE, dwNumberOfSymbols == 0);

                WORD wSizeOfOptionalHeader = GetDataFromPEFile<WORD>(hPEFile);
                AddItemToPEDataTable(S_PEFILEHEADERSIZEOFOPTIONALHEADER + GetHexAndDecStringFromData(wSizeOfOptionalHeader), htiPE);

                AddItemToPEDataTable(S_PEFILEHEADERCHARACTERISTICS + GetHexStringFromData(GetDataFromPEFile<WORD>(hPEFile)), htiPE);

                htiOptionalHeader = AddItemToPEDataTable(S_OPTIONALHEADER, NULL);

                WORD wMagic = GetDataFromPEFile<WORD>(hPEFile);
                string sCheckMagic = GetHexStringFromData(wMagic);
                string sMagic = S_UNKNOWN;
                switch (wMagic) {
                    case (0x10b):
                        sMagic = S_PE32;
                        break;
                    case (0x20b):
                        sMagic = S_PE32PLUS;
                        break;
                    case (0x107):
                        sMagic = S_ROM;
                        break;
                }
                if (sMagic == S_UNKNOWN) {
                    AddItemToPEDataTable(S_OPTIONALHEADERMAGIC + sCheckMagic + S_UNKNOWN, htiOptionalHeader, FALSE);
                }
                else {
                    if (sMagic != S_PE32) {
                        AddItemToPEDataTable(S_OPTIONALHEADERMAGIC + sCheckMagic + sMagic + S_NOTSUPPORTED, htiOptionalHeader);
                    }
                    else {
                        AddItemToPEDataTable(S_OPTIONALHEADERMAGIC + sCheckMagic + sMagic, htiOptionalHeader);

                        AddItemToPEDataTable(S_OPTIONALHEADERADDRESSOFENTRYPOINT + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile, TRUE, 14)), htiOptionalHeader);

                        AddItemToPEDataTable(S_OPTIONALHEADERBASEOFCODE + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiOptionalHeader);

                        AddItemToPEDataTable(S_OPTIONALHEADERBASEOFDATA + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiOptionalHeader);

                        DWORD dwImageBase = GetDataFromPEFile<DWORD>(hPEFile);
                        string sImageBase = GetHexAndDecStringFromData(dwImageBase);
                        if (dwImageBase % 65536 == 0) {
                            AddItemToPEDataTable(S_OPTIONALHEADERIMAGEBASE + sImageBase, htiOptionalHeader);
                        }
                        else {
                            AddItemToPEDataTable(S_OPTIONALHEADERIMAGEBASE + sImageBase + S_NOTAMULTIPLE + S_OF64KIB, htiOptionalHeader, FALSE);
                        }

                        DWORD dwSectionAlignment = GetDataFromPEFile<DWORD>(hPEFile);
                        DWORD dwFileAlignment = GetDataFromPEFile<DWORD>(hPEFile);
                        AddItemToPEDataTable(S_OPTIONALHEADERSECTIONALIGMENT + GetHexAndDecStringFromData(dwSectionAlignment), htiOptionalHeader, dwSectionAlignment >= dwFileAlignment);
                        AddItemToPEDataTable(S_OPTIONALHEADERFILEALIGNMENT + GetHexAndDecStringFromData(dwFileAlignment), htiOptionalHeader, bIsPowerOfTwo(dwFileAlignment) && dwFileAlignment >= 512 && dwFileAlignment <= 65536);

                        WORD wMajorVersion = GetDataFromPEFile<WORD>(hPEFile);
                        WORD wMinorVersion = GetDataFromPEFile<WORD>(hPEFile);
                        BOOL bIsCorrectVersion = (wMinorVersion <= wMajorVersion);
                        AddItemToPEDataTable(S_OPTIONALHEADERMAJOROPERATINGSYSTEMVERSION + GetHexAndDecStringFromData(wMajorVersion), htiOptionalHeader, bIsCorrectVersion);
                        AddItemToPEDataTable(S_OPTIONALHEADERMINOROPERATINGSYSTEMVERSION + GetHexAndDecStringFromData(wMinorVersion), htiOptionalHeader, bIsCorrectVersion);

                        DWORD dwSizeOfImage = GetDataFromPEFile<DWORD>(hPEFile, TRUE, 12);
                        if (dwSizeOfImage % dwSectionAlignment == 0) {
                            AddItemToPEDataTable(S_OPTIONALHEADERSIZEOFIMAGE + GetHexAndDecStringFromData(dwSizeOfImage), htiOptionalHeader);
                        }
                        else {
                            AddItemToPEDataTable(S_OPTIONALHEADERSIZEOFIMAGE + GetHexAndDecStringFromData(dwSizeOfImage) + S_NOTAMULTIPLE + S_OFSECTIONALIGNMENT, htiOptionalHeader, FALSE);
                        }

                        DWORD dwSizeOfHeaders = GetDataFromPEFile<DWORD>(hPEFile);
                        if (dwSizeOfHeaders % dwFileAlignment == 0) {
                            AddItemToPEDataTable(S_OPTIONALHEADERSIZEOFHEADERS + GetHexAndDecStringFromData(dwSizeOfHeaders), htiOptionalHeader);
                        }
                        else {
                            AddItemToPEDataTable(S_OPTIONALHEADERSIZEOFHEADERS + GetHexAndDecStringFromData(dwSizeOfHeaders) + S_NOTAMULTIPLE + S_OFFILEALIGNMENT, htiOptionalHeader, TRUE);
                        }
                        AddItemToPEDataTable( S_OPTIONALHEADERSUBSYSTEM + GetHexStringFromData(GetDataFromPEFile<WORD>(hPEFile, TRUE, 4)), htiOptionalHeader);

                        DWORD NumberOfRvaAndSizes = GetDataFromPEFile<DWORD>(hPEFile, TRUE, 22);
                        AddItemToPEDataTable(S_OPTIONALHEADERNUMBEROFRVAANDSIZES + GetHexAndDecStringFromData(NumberOfRvaAndSizes), htiOptionalHeader, NumberOfRvaAndSizes == 16);

                        htiOptionalHeaderDataDirectory = AddItemToPEDataTable(S_OPTIONALHEADERDATADIRECTORY, htiOptionalHeader);

                        for (size_t i = 0; i < 16; i++) {
                            DWORD dwVirtualAddress = GetDataFromPEFile<DWORD>(hPEFile);
                            DWORD dwSize = GetDataFromPEFile<DWORD>(hPEFile);
                            htiOptionalHeaderDataDirectorySections[i] = AddItemToPEDataTable("[" + std::to_string(i) + "]: " + S_OPTIONALHEADERDATADIRECTORYSECTIONNAMES[i], htiOptionalHeaderDataDirectory,
                                !(((i == 8) && dwSize != 0) ||
                                    ((i == 7 || i == 15) && dwVirtualAddress != 0 && dwSize != 0)));
                            AddItemToPEDataTable(S_OPTIONALHEADERDATADIRECTORYVIRTUALADDRESS + GetHexAndDecStringFromData(dwVirtualAddress), htiOptionalHeaderDataDirectorySections[i]);
                            AddItemToPEDataTable(S_OPTIONALHEADERDATADIRECTORYSIZE + GetHexAndDecStringFromData(dwSize), htiOptionalHeaderDataDirectorySections[i]);
                            PEDataTableItemExpand(htiOptionalHeaderDataDirectorySections[i]);
                            CreateCodeButton(S_OPTIONALHEADERDATADIRECTORYSECTIONNAMES[i], 450, 100 + 40 * unsigned int(i), 200, 30, dwVirtualAddress, dwVirtualAddress + dwSize);
                        }

                        htiSectionHeader = AddItemToPEDataTable(S_SECTIONHEADER, NULL);
                        for (size_t i = 0; i < wNumberOfSections; i++) {
                            HTREEITEM htiSection = AddItemToPEDataTable("[" + std::to_string(i) + "]:", htiSectionHeader);
                            string sName = GetUTF8DataFromPEFile<DWORDLONG>(hPEFile);
                            AddItemToPEDataTable(S_SECTIONHEADERNAME + sName, htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERVIRTUALSIZE + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERVIRTUALADDRESS + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiSection);
                            DWORD dwSizeOfRawData = GetDataFromPEFile<DWORD>(hPEFile);
                            AddItemToPEDataTable(S_SECTIONHEADERSIZEOFRAWDATA + GetHexAndDecStringFromData(dwSizeOfRawData), htiSection);
                            DWORD dwPointerToRawData = GetDataFromPEFile<DWORD>(hPEFile);
                            AddItemToPEDataTable(S_SECTIONHEADERPOINTERTORAWDATA + GetHexAndDecStringFromData(dwPointerToRawData), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERPOINTERTORELOCATIONS + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERPOINTERTOLINENUMBERS + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERNUMBEROFRELOCATIONS + GetHexAndDecStringFromData(GetDataFromPEFile<WORD>(hPEFile)), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERNUMBEROFLINENUMBERS + GetHexAndDecStringFromData(GetDataFromPEFile<WORD>(hPEFile)), htiSection);
                            AddItemToPEDataTable(S_SECTIONHEADERCHARACTERISTICS + GetHexAndDecStringFromData(GetDataFromPEFile<DWORD>(hPEFile)), htiSection);
                            PEDataTableItemExpand(htiSection);
                            CreateCodeButton(sName, 700, 60 + 40 * unsigned int(i), 120, 30, dwPointerToRawData, dwPointerToRawData + dwSizeOfRawData);
                        }
                    }
                }
            }
        }
        else {
            MessageBox(NULL, S_NOTAPEFILE, S_WARNING, MB_ICONWARNING);
        }
    }
    PEDataTableItemExpand(htiDOS);
    PEDataTableItemExpand(htiPE);
    PEDataTableItemExpand(htiOptionalHeader);
    PEDataTableItemExpand(htiOptionalHeaderDataDirectory);
    PEDataTableItemExpand(htiSectionHeader);
    CloseFile(hPEFile);
    return;
}

HTREEITEM MainWindow::AddItemToPEDataTable(string sItem, HTREEITEM hParent, BOOL bCorrectElement) {
    TVINSERTSTRUCT tvins;
    HTREEITEM hme;
    tvins.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_DI_SETITEM | TVIF_PARAM;
    if (!bCorrectElement) {
        tvins.item.mask += TVIF_STATE;
        tvins.item.state = TVIS_BOLD;
        tvins.item.stateMask = TVIS_BOLD;
    }
    tvins.item.pszText = const_cast<char*>(sItem.c_str());
    tvins.hInsertAfter = TVI_ROOT;
    if (hParent == NULL)
    {
        tvins.hParent = TVI_ROOT;
    }
    else
    {
        tvins.hParent = hParent;
    }
    hme = TreeView_InsertItem(hPEDataTable, &tvins);
    return hme;
}

BOOL MainWindow::PEDataTableItemExpand(HTREEITEM hTreeItem) {
    return TreeView_Expand(hPEDataTable, hTreeItem, TVE_EXPAND);
}

inline BOOL MainWindow::bIsPowerOfTwo(int nData) {
    return nData && !(nData & (nData - 1));
}

void MainWindow::CreateCodeButton(string sTitle, int x, int y, int nWidth, int nHeight, LONG lBegOfCode, LONG lEndOfCode) {
    size_t unIndexOfButton = CodeWindows.size();
    HWND hButton = CreateWindowA("button", sTitle.c_str(), WS_VISIBLE | WS_CHILD | ES_CENTER, x, y, nWidth, nHeight, hWnd, (HMENU)IDC_CLICKCODEBUTTON, GetModuleHandle(NULL), NULL);
    CodeButtons.push_back(hButton);
    CodeWindows.push_back(CodeWindow{ this, sTitle, lBegOfCode, lEndOfCode });
    if (lBegOfCode == lEndOfCode) {
        EnableWindow(hButton, FALSE);
    }
    return;
}

void MainWindow::ShowCodeWindow(size_t nButtonIndex) {
    CodeWindows[nButtonIndex].ShowCodeWindow();
    return;
}