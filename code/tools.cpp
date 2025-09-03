#include "tools.h"

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
int readInt(string s, int &p)
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
