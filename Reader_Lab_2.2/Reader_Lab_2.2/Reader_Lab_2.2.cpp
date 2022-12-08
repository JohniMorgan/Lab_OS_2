
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <windows.h>
#include <string>

using namespace std;
void cleanInStream() {
    wcin.clear();
    if (char(wcin.peek()) == L'\n') cin.ignore();
}

int main()
{
    setlocale(LC_ALL, "Rus");
    HANDLE File;
    HANDLE MapFile;
    LPVOID ViewAdress;
    LPTSTR MapName = new wchar_t[MAX_PATH];
    int user = 1;

    wcout << L"Программа - Лабораторная работа №2 часть 2" << endl << L"Выполнил студент 0305 Бутов Евгений" << endl;
    while (user != 0) {
        wcout << L"Введите имя объeкта-сопоставителя:\n>>";
        cleanInStream();
        wcin.getline(MapName, MAX_PATH);
        wcout << L"Открытие файла на чтение....\n";
        if ((MapFile = OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, MapName)) != NULL) {
            if (ViewAdress = MapViewOfFile(MapFile, FILE_MAP_READ, 0, 0, 0)) {
                wcout << L"Всё прошло успешно. Читаю...\n";
                wchar_t* data = (wchar_t*)ViewAdress;
                while (*data) {
                    wcout << *data;
                    data++;
                }
                wcout << L"\n";
                UnmapViewOfFile(ViewAdress);
                CloseHandle(MapFile);
            }
            else {
                wcout << L"Получение проекции завершилось с ошибкой...\n";
                CloseHandle(MapFile);
            }
        }
        else
            wcout << L"Открытие проецируемого файла завершилось с ошибкой...\n";
        wcout << L"Желаете подключаться к другому писателю?\n 1 - Да 0 - Нет\n";
        wcin >> user;
    }
    delete[] MapName;
}

