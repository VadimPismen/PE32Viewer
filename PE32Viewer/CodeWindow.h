#pragma once
#include "BaseWindow.h"
#include "MainWindow.h"
#include <string>
#include <vector>
#include <iomanip>

using std::string;

class MainWindow;

class CodeWindow : public BaseWindow<CodeWindow>
{
public:

    CodeWindow(MainWindow* ParentWindow, string sTitle, LONG lBegOfCode, LONG lEndOfCode);
    ~CodeWindow();

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL ShowCodeWindow();

private:

    HANDLE hCreateHandleOfPEFile();

    BYTE GetBYTEFromPEFile(HANDLE hPEFile);

    string GetHexStringFromData(LONG lData) const;

    string GetHexStringFromData(BYTE BData) const;

    string GetUTF8StringFromBYTE(BYTE BData);

    BOOL OpenPage(LONG dwPage);

    LONG lBegOfCode = 0;
    LONG lEndOfCode = 0;

    HWND hCodeTable = NULL;
    HWND hForwardButton = NULL;
    HWND hBackButton = NULL;
    HWND hCountOfPages = NULL;
    HWND hPageSwitcher = NULL;
    HWND hPageSwitcherButton = NULL;

    MainWindow* ParentWindow;
    char szFile[260] = {};
    string sTitle;


    LONG dwPage = 0;

    DWORD dwCountOfPages = 0;

    VOID AddItemToTable(string sItem, int nLine, int nColumn);
};