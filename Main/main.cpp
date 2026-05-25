#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

#define RESET   "\033[0m"
#define GREEN   "\033[1;32m"
#define AQUA    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define WHITE   "\033[1;37m"

bool IsValidDate(const SYSTEMTIME& st)
{
    if (st.wMonth < 1 || st.wMonth > 12) return false;
    if (st.wHour > 23 || st.wMinute > 59 || st.wSecond > 59) return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (st.wMonth == 2) {
        bool isLeap = (st.wYear % 4 == 0 && st.wYear % 100 != 0) || (st.wYear % 400 == 0);
        if (isLeap) daysInMonth[1] = 29;
    }

    if (st.wDay < 1 || st.wDay > daysInMonth[st.wMonth - 1]) return false;

    return true;
}

void PrintFileTime(const FILETIME& ft)
{
    SYSTEMTIME utc, local;
    FileTimeToSystemTime(&ft, &utc);
    SystemTimeToTzSpecificLocalTime(nullptr, &utc, &local);

    std::cout << std::setfill('0')
        << std::setw(2) << local.wDay << "."
        << std::setw(2) << local.wMonth << "."
        << local.wYear << " "
        << std::setw(2) << local.wHour << ":"
        << std::setw(2) << local.wMinute << ":"
        << std::setw(2) << local.wSecond;
}

bool InputDate(FILETIME& outFt)
{
    std::string input;
    std::cout << WHITE << "  Enter timestamp [DD.MM.YYYY HH:MM:SS] (or press ENTER for NOW): " << RESET;

    std::cin.ignore(std::cin.rdbuf()->in_avail(), '\n');
    std::getline(std::cin, input);

    SYSTEMTIME st{};

    if (input.empty())
    {
        GetLocalTime(&st);
    }
    else
    {
        char dot1, dot2, col1, col2;
        std::stringstream ss(input);

        if (!(ss >> st.wDay >> dot1 >> st.wMonth >> dot2 >> st.wYear
            >> st.wHour >> col1 >> st.wMinute >> col2 >> st.wSecond)
            || dot1 != '.' || dot2 != '.' || col1 != ':' || col2 != ':')
        {
            std::cout << RED << "\n  [!] Error: Invalid format structure! Use DD.MM.YYYY HH:MM:SS\n" << RESET;
            Sleep(2000);
            return false;
        }

        if (!IsValidDate(st))
        {
            std::cout << RED << "\n  [!] Error: Invalid calendar date or time value!\n" << RESET;
            Sleep(2000);
            return false;
        }
    }

    SYSTEMTIME utc;
    if (!TzSpecificLocalTimeToSystemTime(nullptr, &st, &utc))
    {
        std::cout << RED << "\n  [!] Error: Time conversion failed.\n" << RESET;
        Sleep(2000);
        return false;
    }

    return SystemTimeToFileTime(&utc, &outFt);
}

void ShowTimes(HANDLE hFile, const std::string& path)
{
    FILETIME creation, access, write;
    if (!GetFileTime(hFile, &creation, &access, &write)) return;

    system("cls");
    std::cout << AQUA << "------------------------------------------------------\n" << RESET;
    std::cout << WHITE << "  File: " << YELLOW << path << "\n" << RESET;
    std::cout << AQUA << "------------------------------------------------------\n" << RESET;

    std::cout << WHITE << "  1. Created:      " << GREEN; PrintFileTime(creation); std::cout << "\n";
    std::cout << WHITE << "  2. Last Access:  " << GREEN; PrintFileTime(access);   std::cout << "\n";
    std::cout << WHITE << "  3. Last Modify:  " << GREEN; PrintFileTime(write);    std::cout << "\n";

    std::cout << AQUA << "------------------------------------------------------\n\n" << RESET;
}

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    std::string path;
    std::cout << AQUA << "=== Windows Timestamp Editor ===\n" << RESET;
    std::cout << WHITE << "Enter file path (or drag & drop file here): " << RESET;
    std::getline(std::cin, path);

    if (!path.empty() && path.front() == '"' && path.back() == '"') {
        path = path.substr(1, path.length() - 2);
    }

    HANDLE hFile = CreateFileA(
        path.c_str(),
        FILE_WRITE_ATTRIBUTES | FILE_READ_ATTRIBUTES,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << RED << "\n[!] Error: Could not open file. Please check the path.\n" << RESET;
        system("pause");
        return 1;
    }

    while (true)
    {
        ShowTimes(hFile, path);

        std::cout << AQUA << "  [ MENU ]\n" << RESET;
        std::cout << WHITE << "  [1] Change Creation Date\n";
        std::cout << "  [2] Change Last Access Date\n";
        std::cout << "  [3] Change Last Modify Date\n";
        std::cout << "  [4] Set All Timestamps Simultaneously\n";
        std::cout << RED << "  [0] Exit\n\n" << RESET;
        std::cout << WHITE << "  Selection: " << RESET;

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(MAXSHORT, '\n');
            std::cout << RED << "\n  [!] Error: Please enter a valid number.\n" << RESET;
            Sleep(1500);
            continue;
        }

        if (choice == 0) break;

        FILETIME creation{}, access{}, write{};
        GetFileTime(hFile, &creation, &access, &write);
        FILETIME newTime{};

        bool success = false;

        switch (choice)
        {
        case 1:
            if (InputDate(newTime)) success = SetFileTime(hFile, &newTime, &access, &write);
            break;
        case 2:
            if (InputDate(newTime)) success = SetFileTime(hFile, &creation, &newTime, &write);
            break;
        case 3:
            if (InputDate(newTime)) success = SetFileTime(hFile, &creation, &access, &newTime);
            break;
        case 4:
            if (InputDate(newTime)) success = SetFileTime(hFile, &newTime, &newTime, &newTime);
            break;
        default:
            std::cout << RED << "\n  [!] Error: Invalid menu selection.\n" << RESET;
            Sleep(1500);
            continue;
        }

        if (success) {
            std::cout << GREEN << "\n  [+] Timestamps updated successfully!\n" << RESET;
            Sleep(1200);
        }
    }

    CloseHandle(hFile);
    return 0;
}