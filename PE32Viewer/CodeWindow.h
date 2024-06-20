#pragma once
#include "BaseWindow.h"
#include "MainWindow.h"
#include <string>
#include <vector>
#include <iomanip>
#include <locale>

using std::string;

class MainWindow;

// ����� ����, ���������������� ��� ������ � ���� HEX � UTF-8
class CodeWindow : public BaseWindow<CodeWindow>
{
public:
    // ���� �������� ��������� �� ������ ��������; ���������, ������ � ����� ����
    CodeWindow(MainWindow* ParentWindow, string sTitle, LONG lBegOfCode, LONG lEndOfCode);
    ~CodeWindow();

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // �������� ���� ����. ������������ MainWindow ��� ������� �� ������
    BOOL ShowCodeWindow();

private:
    // ������� ���������� �����
    HANDLE hCreateHandleOfPEFile();
    // �������� ���� �� �����
    BYTE GetBYTEFromPEFile(HANDLE hPEFile);
    // �������� 16-�� ������������� ���� LONG
    string GetHexStringFromData(LONG lData) const;
    // �������� 16-�� ������������� ���� BYTE
    string GetHexStringFromData(BYTE BData) const;
    // �������� UTF-8 ������������� ���� BYTE
    string GetUTF8StringFromBYTE(BYTE BData);

    // ������� ��������� ��������
    BOOL OpenPage(LONG dwPage);

    // �������� ������� � �������
    VOID AddItemToTable(string sItem, int nLine, int nColumn);

    // ��������� ��������� ���� ���� (���� �������, ������ ������������ �������)
    HWND hCodeTable = NULL;
    HWND hForwardButton = NULL;
    HWND hBackButton = NULL;
    HWND hCountOfPages = NULL;
    HWND hPageSwitcher = NULL;
    HWND hPageSwitcherButton = NULL;

    // ��������� �� ������������ ����
    MainWindow* ParentWindow;
    // ��� ������������ �����. ��������� �� ������������� ����.
    char szFile[260] = {};
    // ��� ����
    string sTitle;
    // ������ � ����� ����
    LONG lBegOfCode = 0;
    LONG lEndOfCode = 0;

    // ������� ��������. ��� ������������ ��� �������� ����
    LONG dwPage = 0;
    // ���������� �������
    DWORD dwCountOfPages = 0;
    string sCountOfPages = {};

    // �����, � �������� ���������� ������������ �����.
    LONG lAlignedBegOfCode = 0;
    // ���������� ������, �� ������� ������ ���� ������ �������� �� ����� ������� ������, �������� 16.
    // ����� ���� ��� ����� ��������� ������������� ������
    BYTE nTailOfBytes = 0;
};