#include "tools.h"
#include <cstdlib>
namespace fs = std::filesystem;

int readInt() 
{
    int res = 0; char c = getchar(); int f = 1;
    while (c > '9' || c < '0') {
        if (c == '-') f = -1;
        c = getchar();
    } while (c <= '9' && c >= '0') {
        res = (res << 3) + (res << 1) + (c ^ 48);
        c = getchar();
    }
    return res * f;
}
int readInt(std::string s, int &p)
{
    int res = 0; char c = s[p]; int f = 1;
    while (c > '9' || c < '0') {
        if (c == '-') f = -1;
        c = s[++p];
    } while (c <= '9' && c >= '0') {
        res = (res << 3) + (res << 1) + (c ^ 48);
        c = s[++p];
    }
    return res * f;
}

char readChar() {
    char c = getchar();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = getchar();
    return c;
}
void Swap(int &a, int &b) 
{
    int temp = a;
    a = b;
    b = temp;
    return ;
}
void shuffle(int a[], int n)// 0 ~ n-1
{
    for(int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        Swap(a[i], a[j]);
    }
    return ;
}
// Print colored string s in console, color can be "red", "green", "blue", "yellow", or others for default color
void printColor(std::string s, std::string color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if(color == "red") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    else if(color == "green") SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else if(color == "blue") SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    else if(color == "yellow") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("%s", s.c_str());
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ;
}

int chooseNum(int start, int end) // [start, end]
{
    int num = readInt();
    while(num < start || num > end) 
    {
        printf("Invalid number! Please choose again!\n");
        num = readInt();
    }
    return num;
}

// Display menu options and get user choice
// The indexes will be automatically assigned from 1 to n
int ChooseOptions(const std::string menu[]) 
{
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
    int choice, len = 0;
    while(menu[len] != "")++len;
    for(int i = 0; i < len; ++i) 
    {
        printf("%d. %s\n", i + 1, menu[i].c_str());
    }
    choice = chooseNum(1, len);
    return choice;
}

// Function to get the directory of the executable
std::string getExePath() {
    char buffer[MAX_PATH];
    // get the exe path like C:\Project\build\myapp.exe
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    // find the last "\" or "/" and extract the directory part
    size_t pos = fullPath.find_last_of("\\/");
    return fullPath.substr(0, pos);
}

std::string GetRootPath() {
    std::string exePath = getExePath();
    fs::path p(exePath);
    // Assuming the executable is in a subdirectory of the root project directory
    fs::path rootPath = p.parent_path(); 
    return rootPath.string();
}

