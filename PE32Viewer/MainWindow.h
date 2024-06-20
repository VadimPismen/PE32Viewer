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

// Класс основного окна программы
class MainWindow : public BaseWindow<MainWindow>
{
public:
    // Имя открываемого файла
    char szFile[260] = {};

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Получение данных из файла в виде текста UTF-8
    template <typename T>
    static string GetUTF8DataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    // Получение нужного типа данных из файла
    template<typename T>
    T GetDataFromPEFile(HANDLE hPEFile, BOOL ReadFromCurrentPose = TRUE, LONG lDistanceToMove = 0, PLONG lpDistanceToMoveHigh = NULL);

    // Получение 16-го представления данных
    template<typename T>
    string GetHexStringFromData(T TData, BOOL bSetFill = TRUE) const;

    // Получение 16-го и 10-го представления данных
    template<typename T>
    string GetHexAndDecStringFromData(T TData) const;

    // Получение сигнатуры из строки
    // (то есть он добавляет кавычки, а непечатаемые символы заменяет на HEX)
    string GetSignatureFromString(string sData) const;

    // Получение строки из времени
    string GetStringFromTime(const time_t* tTime) const;

private:
    // Создание меню с кнопкой "Открыть файл"
    VOID CreateReaderMenu();
    // Открыть файл с помощью диалога
    BOOL OpenFileWithDialogue();
    // Создать файловый поток
    HANDLE hCreateHandleOfPEFile();
    // Закрыть файловый поток
    BOOL CloseFile(HANDLE hPEFile);

    // Динамический массив кнопок и окон, связанных с просмотром кода секции
    std::vector<HWND> CodeButtons;
    std::vector<CodeWindow> CodeWindows;

    // Дескриптор изображения названия файла и древовидного представления PE файла.
    HWND hPEFileName = 0;
    HWND hPEDataTable = 0;

    // Заполнение древа данными файла
    void FillPEDataTable();
    // Добавление элемента в древо
    HTREEITEM AddItemToPEDataTable(string sItem, HTREEITEM hParent = NULL, BOOL bCorrectElement = TRUE);
    // Раскрытие главных элементов древа
    BOOL PEDataTableItemExpand(HTREEITEM hTreeItem);

    // Дескрипторы главных элементов древа
    HTREEITEM htiDOS = 0;
    HTREEITEM htiPE = 0;
    HTREEITEM htiOptionalHeader = 0;
    HTREEITEM htiOptionalHeaderDataDirectory = 0;
    HTREEITEM htiOptionalHeaderDataDirectorySections[16] = {};
    HTREEITEM htiSectionHeader = 0;

    // Структура, используемая для создания диалога открытия файла
    OPENFILENAME ofn = {};

    // Создание кнопки, открывающей окно кода секции
    void CreateCodeButton(string sTitle, int x, int y, int nWidth, int nHeight, LONG lBegOfCode, LONG lEndOfCode);
    
    // Показ окна кода секции
    void ShowCodeWindow(size_t nButtonIndex);

    __declspec(deprecated) BOOL CheckEndOfSection(HANDLE hPEFile, DWORD dwSizeOfRawData, DWORD dwPointerToRawData);
    
    // Узнать, является ли целочисленное степенью двойки
    BOOL bIsPowerOfTwo(int nData);
};