
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <windows.h>
#include <string>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus");
    LPTSTR FilePath = new wchar_t[MAX_PATH];
    HANDLE File;
    HANDLE MapFile = NULL;
    LPVOID ViewAdress;
    wcout << L"Программа - Лабораторная работа №2 часть 2" << endl << L"Выполнил студент 0305 Бутов Евгений" << endl;
    wcout << L"Введите полный путь к создаваемому файлу:\n" << L">>";
    //Шаг 1. Считываем путь до файла и пытаемся его открыть.
    wcin.getline(FilePath, MAX_PATH);
    wcout << L"Открытие файла на запись....\n";
    if ((File = CreateFile(FilePath, GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, NULL, 0)) != INVALID_HANDLE_VALUE) {
        wcout << L"Файл успешно открылся\nВведите имя нового сопоставителя\n";
        wcin.getline(FilePath, MAX_PATH);
        if ((MapFile = CreateFileMapping(File, NULL, PAGE_READWRITE, 0, 64, FilePath)) != NULL) {
            //Шаг 3.
            if ((ViewAdress = MapViewOfFile(MapFile, FILE_MAP_WRITE, 0, 0, 0)) != NULL) {
                wcout << L"Сопоставление успешно развёрнуто. Введите данные для записи в файл\n";
                wcin.getline((wchar_t*)ViewAdress, 64);
                wcout << L"Данные записаны. Откройте приложение-чтеца.\n";
                wcout << L"Как только закончите работу со чтецом, нажмите Enter\n";
                getchar();

                UnmapViewOfFile(ViewAdress);
                CloseHandle(MapFile);
                CloseHandle(File);
            }
            else {
                wcout << L"Проецирование файла в ВАП завершилось с ошибкой..." << GetLastError();
                CloseHandle(MapFile);
                CloseHandle(File);
            }
        }
        else {
            CloseHandle(File);
            wcout << L"Открытие проецируемого файла завершилось с ошибкой... " << GetLastError();
        }
    }
    else {
        wcout << L"Открытие файла не удалось... " << GetLastError() << L"\n";
    }
    delete[] FilePath;
}

