// Реализация задания №1 Лабораторной работы 2
#include <iostream>
#include <limits>
#include <cmath>
#include <stdio.h>
#include <windows.h>
#include <string>

using namespace std;

/*Функция вывода главного меню*/
void PrintMainMenu();
/*Функция конвертации ввода пользователя в число*/
int InputAsNumber();
/*Пункт меню 1: Получение информации о вычислительной системе*/
void AboutSystem();
/*Пункт меню 2: Определение статуса виртуальной памяти*/
void MemoryStatus();
/*Пункт меню 3: Определение статуса конкретного участка памяти*/
/*Адрес задаётся пользователем*/
void MemoryCurrentStatus();
/*Доп функция определения параметров защиты*/
void MemProtectionList(DWORD memory);
/*Пункт меню 4: Резервирование памяти*/
/*Реализует как автоматический режим, так и ручной*/
void ReserveMemory();
/*Пункт меню 5: Резервирование памяти вместе с выделением физической памяти*/
/*Реализуется как автоматический режим, так и ручной*/
void ReserveAndCommitMemory();
/*Пункт меню 6: Запись в зарезервированную и зафиксированную память*/
void WriteToVMA(); //Virtual Memory Area
/*Пункт меню 7: Установка прав доступа и их проверка по адресу*/
void SetProtectAndCheck();
/*Пункт меню 8: Высвобождение памяти по адресу*/
void FreeByAdress();
void cleanInStream() {
    wcin.clear();
    if (wchar_t(wcin.peek()) == L'\n') wcin.ignore();
}


int main()
{
    setlocale(LC_ALL, "Russian");
    void (*menu_item[8])() = { AboutSystem, MemoryStatus, MemoryCurrentStatus, 
        ReserveMemory, ReserveAndCommitMemory, WriteToVMA, SetProtectAndCheck, FreeByAdress };
    int user;
    wcout << L"Программа - Лабораторная работа №2 часть 1" << endl << L"Выполнил студент 0305 Бутов Евгений" << endl;
    do {
        wcout << L"---------------------------------------------------------" << endl;
        PrintMainMenu();
        user = InputAsNumber();
        system("cls");
        if (user != 0) menu_item[user - 1]();
    } while (user != 0);
    
}

void PrintMainMenu() {
    wcout << L"Пожалуйста, выберите опцию меню:" << endl;
    wcout << L"1 - Получение информации о вычислительной системе" << endl;
    wcout << L"2 - Определение статуса виртуальной памяти" << endl;
    wcout << L"3 - Определение статуса учатска памяти по адресу" << endl;
    wcout << L"4 - Резервирование участка памяти ВАП по адресу" << endl;
    wcout << L"5 - Резервирование и выделение физической памяти под участок памяти ВАП по адресу" << endl;
    wcout << L"6 - Запись в зарезервированную и зафиксирванную память" << endl;
    wcout << L"7 - Установка новых прав доступа и проверка по адресу" << endl;
    wcout << L"8 - Освобождение памяти по адресу" << endl;
    wcout << L"0 - Выход из программы" << endl;
}

int InputAsNumber() {
    string user;
    int in = -1;
    bool flag = true;
    do {
        wcout << L">>";
        cleanInStream();
        getline(cin, user);
        try {
            in = stoi(user);
            flag = true;
        }
        catch (const std::invalid_argument) {
            flag = false;
            wcout << L"Неверный формат команды. Ожидалось натуральное число!" << endl;
        }

    } while (!flag);
    return in;
}

void AboutSystem() {
    SYSTEM_INFO information;

    GetSystemInfo(&information);
    wcout << L"Получение информации о системе..." << endl;
    wcout << L"Тип процессора: ";
    switch (information.wProcessorArchitecture) {
    case 9:
        wcout << L"x64 (семейства AMD или Intel)";
        break;
    case 5:
        wcout << L"ARM";
        break;
    case 12:
        wcout << L"ARM64";
        break;
    case 6:
        wcout << L"на базе Intel Itanium";
        break;
    case 0:
        wcout << L"Intel x86";
        break;
    default:
        wcout << L"Невозможно определить!";
    }
    wcout << endl;
    wcout << L"Размер страницы, Это значение использует VirtualAlloc(): " << information.dwPageSize << endl;
    wcout << endl;
    wcout << L"Указатели на наименьший и наибольший адреса памяти" << endl << L"доступные приложениям и библиотекам динамической компановки DLL : ";
    wcout << information.lpMinimumApplicationAddress << L" " << information.lpMaximumApplicationAddress << endl;
    wcout << L"Процессоры сконфигурированные в системе (номера): " << endl;
    for (int i = 0; i < 32; i++) if (information.dwActiveProcessorMask) {
        wcout << i << L" ";
    }
    wcout << endl;
    wcout << L"Количество логических процессоров в выбранной группе: " << information.dwNumberOfProcessors << endl;
    wcout << L"Степень детализации для начального адреса, по которому можно выделить виртуальную память: " << information.dwAllocationGranularity << endl;

    return;
}

void MemoryStatus() {
    MEMORYSTATUS mem;

    GlobalMemoryStatus(&mem);
    wcout << L"Загрузка статуса виртуальной памяти..." << endl;

    wcout << L"Количество используемой физической памяти: " << mem.dwMemoryLoad << L"%" << endl;
    wcout << L"Количество фактической физической памяти: " << mem.dwTotalPhys << L" Байт" << endl;
    wcout << L"Количество доступной физической памяти: " << mem.dwAvailPhys << L" Байт" << endl;
    wcout << L"Текущий размер файла подкачки: " << mem.dwTotalPageFile << L" Байт" << endl;
    wcout << L"Количество свободной памяти для файлов подкачки: " << mem.dwAvailPageFile << L" Байт" << endl;
    wcout << L"Размер части виртуального адресного пространства" << endl;
    wcout << L"вызывающего процесса в пользовательском режиме: " << mem.dwTotalVirtual << L" Байт" << endl;
    wcout << L"Объем незанятой и незафиксированной памяти, находящейся в данный момент" << endl;
    wcout << L"в части виртуального адресного пространства вызывающего процесса в пользовательском режиме: " << endl;
    wcout << mem.dwAvailVirtual << L" Байт" << endl;

    return;
}

void MemProtectionList(DWORD memory) {
    switch (memory) {
    case PAGE_EXECUTE:
        wcout << L"Разрешен доступ на выполнение." << endl;
        break;
    case PAGE_EXECUTE_READ:
        wcout << L"Разрешен доступ на выполнение или чтение." << endl;
        break;
    case PAGE_EXECUTE_READWRITE:
        wcout << L"Разрешен доступ на выполнение, только чтение или чтение/запись." << endl;
        break;
    case PAGE_EXECUTE_WRITECOPY:
        wcout << L"Разрешен доступ на выполнение, только чтение или запись/копирование." << endl;

    case PAGE_NOACCESS:
        wcout << L"Доступ запрещён." << endl;
        break;
    case PAGE_READONLY:
        wcout << L"Доступ только для чтения." << endl;
        break;
    case PAGE_READWRITE:
        wcout << L"Доступ на чтение и запись." << endl;
        break;
    case PAGE_WRITECOPY:
        wcout << L"Доступ только на чтение или на копирование." << endl;
    }
    if (memory | PAGE_GUARD) wcout << L"Страницы являются защищёнными." << endl;
    if (memory | PAGE_NOCACHE) wcout << L"Страницы не кэшируется." << endl;
} 

void MemoryCurrentStatus() {
    LPVOID adress;
    MEMORY_BASIC_INFORMATION mem;
    wcout << L"Пожалуйста, введите адрес области ВАП: " << endl << L">>";
    wcin >> adress;
    if (VirtualQuery(adress, &mem, MAX_PATH)) {
        wcout << L"Загрузка информации...." << endl;
        wcout << L"Базовый адрес области ВАП: " << mem.BaseAddress << endl;
        wcout << L"Базовый адрес выделенной области в физ. памяти: " << mem.AllocationBase << endl;
        wcout << L"Исходные атрибуты защиты страниц: " << endl;
        MemProtectionList(mem.AllocationProtect);
        wcout << L"Размер области: " << mem.RegionSize << L" Байт" << endl;
        wcout << L"Состояние области: " << endl;
        switch (mem.State) {
        case MEM_COMMIT:
            wcout << L"Готовы к использованию." << endl;
            break;
        case MEM_FREE:
            wcout << L"Страницы свободны." << endl;
            break;
        case MEM_RESERVE:
            wcout << L"Страницы зарезервированы." << endl;
            break;
        }
        wcout << L"Текущая защита доступа: " << endl;
        MemProtectionList(mem.Protect);
    }
    else {
        if (GetLastError() == ERROR_INVALID_PARAMETER) wcout << L"Некорректный адрес. Ожидалось 8-значное 16-чное число!" << endl;
        wcout << L"Возврат в главное меню...." << endl;
    }
}

void ReserveMemory() {
    wcout << L"Внимание! Эта функция не выделяет физической памяти." << endl;
    wcout << L"Что-бы выделить физическую память для участка ВАП, используется опцию 5 меню." << endl;
    wcout << L"Пожалуйста, выберите режим работы:" << endl;
    LPVOID mem;

    wcout << L"1 - Автоматическое резервирование" << endl << L"2 - Ручное резервирование" << endl << L"Другое число - Возврат в меню" << endl << L">>";
    switch(InputAsNumber()) {
    case 1:
        wcout << L"Память будет выделена с правами доступа на чтение/запись" << endl;
        if (mem = VirtualAlloc(NULL, 4096, MEM_RESERVE, PAGE_READWRITE))
            wcout << L"Была зарезервирована область памяти по базовому адресу " << mem << endl;
        else wcout << L"Что-то пошло не так..." << GetLastError() << endl;
        break;
    case 2:
        wcout << L"Введите адрес памяти которую вы хотите зарезервировать" << endl;
        wcout << L"Память будет выделена с правом доступа на чтение/запись" << endl << L">>";
        wcin >> mem;
        if (VirtualAlloc(mem, 4096, MEM_RESERVE, PAGE_READWRITE))
            wcout << L"Память по адресу " << mem << L" была успешно зарезервирована." << endl;
        else if (GetLastError() == 487)
            wcout << L"Вы пытаетесь зарезервировать уже зарезервированную область памяти!" << endl;
    }
}

void ReserveAndCommitMemory() {
    wcout << L"Пожалуйста, выберите режим работы:" << endl;
    LPVOID mem;

    wcout << L"1 - Автоматическое резервирование" << endl << L"2 - Ручное резервирование" << endl << L"Другое число - Возврат в меню" << endl << L">>";
    switch (InputAsNumber()) {
    case 1:
        wcout << L"Память будет выделена с правами доступа на чтение/запись" << endl;
        if (mem = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))
            wcout << L"Была зарезервирована область памяти по базовому адресу " << mem << endl;
        else if(GetLastError() == 487) {
            if (VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE))
                wcout << L"Память по адресу " << mem << L" была успешно зарезервирована." << endl;
            else
                wcout << L"Что-то пошло не так... " << GetLastError() << "\n";
        }
        break;
    case 2:
        wcout << L"Введите адрес памяти которую вы хотите зарезервировать" << endl;
        wcout << L"Память будет выделена с правом доступа на чтение/запись" << endl << L">>";
        wcin >> mem;
        if (VirtualAlloc(mem, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))
            wcout << L"Память по адресу " << mem << L" была успешно зарезервирована." << endl;
        else if (GetLastError() == 487) {
            if (VirtualAlloc(mem, 4096, MEM_COMMIT, PAGE_READWRITE))
                wcout << L"Память по адресу " << mem << L" была успешно зарезервирована." << endl;
            else
                wcout << L"Что-то пошло не так... " << GetLastError() << "\n";
        }
    }
}

void WriteToVMA() {
    wcout << L"Введите область памяти с которой хотите начать запись:" << endl;
    wcout << L">>";

    LPVOID mem;
    MEMORY_BASIC_INFORMATION aboutMem;

    wcin >> mem;
    cleanInStream();
    VirtualQuery(mem, &aboutMem, MAX_PATH);
    if (aboutMem.State != MEM_COMMIT) {
        wcout << L"Память не была подтверждена. Возможно она является свободной или зарезервированной." << endl;
        wcout << L"Пожалуйста, зарезервируйте и зафиксируйте память пунктом 5 главного меню." << endl;
        wcout << L"Возврат в главное меню..." << endl;
    }
    else
    {
        if (!(aboutMem.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE_READWRITE))) {
            wcout << L"Эта память не является выделенной для записи в неё данных." << endl << L"Пожалуйста, измените права доступа и повторите попытку" << endl;
            wcout << L"Возврат в главное меню..." << endl;
        }
        else {
            SIZE_T length = aboutMem.RegionSize;
            wcout << L"Пожалуйста, введите информацию которую вы хотите записать (Строка)" << endl;
            wcout << L">>";

            wcin.getline((wchar_t*)mem, length);
            wchar_t* pchar = (wchar_t*)mem;
            wcout << L"Вы ввели : ";
            while (*pchar != L'\0') {
                wcout << *pchar;
                pchar++;
            }
            wcout << endl;
            wcout << L"Запись закончилась по адресу: " << (void*)pchar << endl;
        }
    }
}

void SetProtectAndCheck() {
    LPVOID adress;
    MEMORY_BASIC_INFORMATION aboutMem;
    wcout << L"Пожалуйста, введите адрес области ВАП: " << endl << L">";
    wcin >> adress;

    DWORD newProtect = 0;
    PDWORD oldProtect = new DWORD;
    int exe;
    int access;
    wcout << L"Выберите новый доступ к памяти:" << endl;
    wcout << L"1 - Чтение" << endl;
    wcout << L"2 - Чтение и запись" << endl;
    wcout << L"3 - Запись и копирование" << endl;
    wcout << L"0 - Закрыть доступ на чтение/запись" << endl << ">>";
    access = InputAsNumber();
    wcout << L"Должна ли информация в этой памяти быть исполняемой?" << endl;
    wcout << L"1 - Да" << endl;
    wcout << L"0 - Нет" << endl << ">>";
    exe = InputAsNumber();
    if (exe == 1) {
       switch (access) {
       case 1:
           newProtect = PAGE_EXECUTE_READ;
           break;
       case 2:
           newProtect = PAGE_EXECUTE_READWRITE;
           break;
       case 3:
           newProtect = PAGE_EXECUTE_WRITECOPY;
           break;
       case 0:
           newProtect = PAGE_EXECUTE;
           break;
       }
    }
    else if (exe == 0) {
        switch (access) {
        case 1:
            newProtect = PAGE_READONLY;
            break;
        case 2:
            newProtect = PAGE_READWRITE;
            break;
        case 3:
            newProtect = PAGE_WRITECOPY;
            break;
        case 0:
            newProtect = PAGE_NOACCESS;
        }
    }      
    VirtualQuery(adress, &aboutMem, MAX_PATH);
    VirtualProtect(adress, aboutMem.RegionSize, newProtect, oldProtect);
    wcout << L"Старый тип защиты:" << endl;
    MemProtectionList(*oldProtect);
    wcout << L"Заменён на новый:" << endl;
    MemProtectionList(newProtect);
    delete oldProtect;
}

void FreeByAdress() {
    wcout << L"Пожалуйста, введите адрес памяти для освобождения" << endl << ">>";
    LPVOID adress;
    wcin >> adress;
    MEMORY_BASIC_INFORMATION aboutMem;
    VirtualQuery(adress, &aboutMem, MAX_PATH);
    SetLastError(0);
    VirtualFree(aboutMem.AllocationBase, 0, MEM_RELEASE);
    if (GetLastError()) wcout << L"Возникла ошибка при высвобождении памяти!" << L" Код ошибки " << GetLastError() << endl;
    else wcout << L"Память успешно освобождена" << endl;
}

