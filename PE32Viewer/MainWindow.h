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

// ����� ��������� ���� ���������
class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ��� ������������ �����
    char szFile[260] = {};

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // ��������� ������ �� ����� � ���� ������ UTF-8
    template <typename T>
    static string GetUTF8DataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    // ��������� ������� ���� ������ �� �����
    template<typename T>
    T GetDataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    // ��������� 16-�� ������������� ������
    template<typename T>
    string GetHexStringFromData(T TData, BOOL bSetFill = TRUE) const;

    // ��������� 16-�� � 10-�� ������������� ������
    template<typename T>
    string GetHexAndDecStringFromData(T TData) const;

    // ��������� ��������� �� ������
    // (�� ���� �� ��������� �������, � ������������ ������� �������� �� HEX)
    string GetSignatureFromString(string sData) const;

    // ��������� ������ �� �������
    string GetStringFromTime(const time_t* tTime) const;

private:
    // �������� ���� � ������� "������� ����"
    VOID CreateReaderMenu();
    // ������� ���� � ������� �������
    BOOL OpenFileWithDialogue();
    // ������� �������� �����
    HANDLE hCreateHandleOfPEFile();
    // ������� �������� �����
    BOOL CloseFile(HANDLE hPEFile);

    // ������������ ������ ������ � ����, ��������� � ���������� ���� ������
    std::vector<HWND> CodeButtons;
    std::vector<CodeWindow> CodeWindows;

    // ���������� ����������� �������� ����� � ������������ ������������� PE �����.
    HWND hPEFileName = 0;
    HWND hPEDataTable = 0;

    // ���������� ����� ������� �����
    void FillPEDataTable();
    // ���������� �������� � �����
    HTREEITEM AddItemToPEDataTable(string sItem, HTREEITEM hParent = NULL, BOOL bCorrectElement = TRUE);
    // ��������� ������� ��������� �����
    BOOL PEDataTableItemExpand(HTREEITEM hTreeItem);

    // ����������� ������� ��������� �����
    HTREEITEM htiDOS = 0;
    HTREEITEM htiPE = 0;
    HTREEITEM htiOptionalHeader = 0;
    HTREEITEM htiOptionalHeaderDataDirectory = 0;
    HTREEITEM htiOptionalHeaderDataDirectorySections[16] = {};
    HTREEITEM htiSectionHeader = 0;

    // ���������, ������������ ��� �������� ������� �������� �����
    OPENFILENAME ofn = {};

    // �������� ������, ����������� ���� ���� ������
    void CreateCodeButton(string sTitle, int x, int y, int nWidth, int nHeight, LONG lBegOfCode, LONG lEndOfCode);
    
    // ����� ���� ���� ������
    void ShowCodeWindow(size_t nButtonIndex);

    __declspec(deprecated) BOOL CheckEndOfSection(HANDLE hPEFile, DWORD dwSizeOfRawData, DWORD dwPointerToRawData);
    
    // ������, �������� �� ������������� �������� ������
    BOOL bIsPowerOfTwo(int nData);
};