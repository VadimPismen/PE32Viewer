#pragma once
#include "BaseWindow.h"
#include <commdlg.h>
#include <CommCtrl.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "PE32ViewerConstants.h"
#include <ctime>
#include "CodeWindow.h"

using std::string;

class CodeWindow;

class MainWindow : public BaseWindow<MainWindow>
{
public:
    char szFile[260] = {};

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    template <typename T>
    static string GetUTF8DataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    template<typename T>
    T GetDataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    template<typename T>
    string GetHexStringFromData(T TData, BOOL bSetFill = TRUE) const;

    template<typename T>
    string GetHexAndDecStringFromData(T TData) const;

    string GetSignatureFromString(string sData) const;
    string GetStringFromTime(const time_t* tTime) const;

private:

    char szOldFile[260] = {};

    std::vector<HWND> CodeButtons;
    std::vector<CodeWindow> CodeWindows;

    HWND hPEFileName = 0;
    HWND hPEDataTable = 0;

    HTREEITEM htiDOS = 0;
    HTREEITEM htiPE = 0;
    HTREEITEM htiOptionalHeader = 0;
    HTREEITEM htiOptionalHeaderDataDirectory = 0;
    HTREEITEM htiOptionalHeaderDataDirectorySections[16] = {};
    HTREEITEM htiSectionHeader = 0;

    OPENFILENAME ofn = {};

    void CreateCodeButton(string sTitle, int x, int y, int nWidth, int nHeight, LONG lBegOfCode, LONG lEndOfCode);
    void ShowCodeWindow(size_t nButtonIndex);
    VOID CreateReaderMenu();
    BOOL OpenFileWithDialogue();
    BOOL CloseFile(HANDLE hPEFile);
    HANDLE hCreateHandleOfPEFile();
    void FillPEDataTable();
    HTREEITEM AddItemToPEDataTable(string sItem, HTREEITEM hParent = NULL, BOOL bCorrectElement = TRUE);
    BOOL PEDataTableItemExpand(HTREEITEM hTreeItem);
    BOOL bIsPowerOfTwo(int nData);
};