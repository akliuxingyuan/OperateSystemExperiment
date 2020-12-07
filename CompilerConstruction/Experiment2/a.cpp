#include <cmath>
#include <string>
#include <iostream>

using namespace std;

string str;
int pos;
bool flag;

char judge(char ch)
{
    if (ch >= '0' && ch <= '9')
        return 'n';
    else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        return 'a';
    else
        return ch;
}

void error(char ch)
{
    if (flag)
        return;
    else
        flag = false;
    cout << endl
         << "error:" << ch << ",position:" << pos;
}

void match(char kind, char ch)
{
    cout << kind << ": " << ch << endl;
    pos++;
}

void atom()
{
    if (!flag)
        return;
    char ch = judge(str[pos]);
    if (ch == 'n')
        match('n', str[pos]);
    else if (ch == 'a')
        match('a', str[pos]);
    else if (ch == '$')
        cout << "accept" << endl;
    else
        error(ch);
}

void list()
{
    if (!flag)
        return;
    match('l', '(');
    cout << " ";
    lexp_seq1();
    if (str[pos] == ')')
        match('r', ')');
    else
        error(str[pos]);
}

void lexp()
{
    if (!flag)
        return;
    char ch = judge(str[pos]);
    if (ch == 'n' || ch == 'a')
    {
        cout << " ";
        atom();
    }
    else if (ch == '(')
    {
        cout << " ";
        list();
    }
    else if (ch == '$')
        cout << "accept" << endl;
    else
        error(ch);
}

void lexp_seq1()
{
    if (!flag)
        return;
    char ch = judge(str[pos]);
    if (ch == 'n' || ch == 'a' || ch == '(')
    {
        cout << " ";
        lexp();
        cout << " ";
        lexp_seq1();
    }
    else if (ch == ')')
        match('r', ')');
    else if (ch == '$')
        cout << "accept" << endl;
    else
        error(ch);
}

int main()
{
    pos = 0;
    flag = true;
    cin >> str;
    str += '$';
    lexp_seq1();
    return 0;
}