#include <iostream>
using namespace std;

int pos;
string str, s;

void error();
void E(int x);
void EE(int x);
void T(int x);
void TT(int x);
void F(int x);

void error()
{
    cout << "error :" << pos + 1 << endl;
    exit(0);
}

void E(int x)
{

    if (pos == str.size())
        return;

    T(x + 1);
    EE(x + 1);
}

void EE(int x)
{

    if (pos == str.size())
        return;
    if (str[pos] == '+')
    {
        for (int i = 1; i <= x; i++)
            cout << ' ';
        cout << "E` +" << endl;
        pos++;
        T(x + 1);
        EE(x + 1);
    }
    else
        return;
}

void T(int x)
{

    if (pos == s.size())
        return;
    F(x + 1);
    TT(x + 1);
}

void TT(int x)
{

    if (pos == s.size())
        return;
    if (str[pos] == '*')
    {
        for (int i = 1; i <= x; i++)
            cout << ' ';
        cout << "T` *" << endl;
        pos++;
        F(x + 1);
        TT(x + 1);
    }
    else
        return;
}

void F(int x)
{

    if (pos == s.size())
        return;
    if (str[pos] == '(')
    {
        pos++;
        for (int i = 1; i <= x; i++)
            cout << ' ';
        cout << "(:(" << endl;
        E(x + 1);
        if (str[pos] == ')')
        {
            pos++;
            for (int i = 1; i <= x; i++)
                cout << ' ';
            cout << "):)" << endl;
            return;
        }
        else
            error();
    }
    else if (str[pos] >= '0' && str[pos] <= '9')
    {
        pos++;
        for (int i = 1; i <= x; i++)
            cout << ' ';
        cout << "id: " << str[pos - 1] << endl;
    }
    else
        error();
}

int main()
{
    cin >> str;
    s = str;
    E(0);
    if (s.size() == pos)
        cout << "accept" << endl;
    else
        error();
}
