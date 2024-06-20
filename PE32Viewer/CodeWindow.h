#pragma once
#include "BaseWindow.h"
#include "MainWindow.h"
#include <string>
#include <vector>
#include <iomanip>
#include <locale>

using std::string;

class MainWindow;

// Класс окна, предоставляющего код секций в виде HEX и UTF-8
class CodeWindow : public BaseWindow<CodeWindow>
{
public:
    // Окно получает указатель на своего родителя; заголовок, начало и конец кода
    CodeWindow(MainWindow* ParentWindow, string sTitle, LONG lBegOfCode, LONG lEndOfCode);
    ~CodeWindow();

    PCSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Показать окно кода. Используется MainWindow при нажатии на кнопки
    BOOL ShowCodeWindow();

private:
    // Создать дескриптор файла
    HANDLE hCreateHandleOfPEFile();
    // Получить байт из файла
    BYTE GetBYTEFromPEFile(HANDLE hPEFile);
    // Получить 16-ое представление типа LONG
    string GetHexStringFromData(LONG lData) const;
    // Получить 16-ое представление типа BYTE
    string GetHexStringFromData(BYTE BData) const;
    // Получить UTF-8 представление типа BYTE
    string GetUTF8StringFromBYTE(BYTE BData);

    // Открыть указанную страницу
    BOOL OpenPage(LONG dwPage);

    // Добавить элемент в таблицу
    VOID AddItemToTable(string sItem, int nLine, int nColumn);

    // Описатели элементов окна кода (сама таблица, кнопки переключения страниц)
    HWND hCodeTable = NULL;
    HWND hForwardButton = NULL;
    HWND hBackButton = NULL;
    HWND hCountOfPages = NULL;
    HWND hPageSwitcher = NULL;
    HWND hPageSwitcherButton = NULL;

    // Указатель на родительское окно
    MainWindow* ParentWindow;
    // Имя открываемого файла. Передаётся от родительского окна.
    char szFile[260] = {};
    // Имя окна
    string sTitle;
    // Начало и конец кода
    LONG lBegOfCode = 0;
    LONG lEndOfCode = 0;

    // Текущая страница. Она запоминается при закрытии окна
    LONG dwPage = 0;
    // Количество страниц
    DWORD dwCountOfPages = 0;
    string sCountOfPages = {};

    // Адрес, с которого начинается перечисление ячеек.
    LONG lAlignedBegOfCode = 0;
    // Количество байтов, на которое начало кода секции отдалено от более раннего адреса, кратного 16.
    // Нужно лишь для более красивого представления данных
    BYTE nTailOfBytes = 0;
};