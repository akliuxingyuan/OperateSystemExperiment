/*
 * @Author: akliuxingyuan
 * @Date: 2021-01-01 16:41:32
 * @Description: 文件系统模拟
 */
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;

/**
 * @brief 文件
 */
typedef struct UFD
{
    char name[10]; 
    int attribute;
    char lock[10]; 
    int space;
    string content;
    int p0[10];    
    int *p1;       
    int (*p2)[100];
    struct UFD *next;
} UFD;

/**
 * @brief 目录
 */
typedef struct DIR
{
    char name[10];
    int space;
    DIR *pre;
    DIR *next;
    UFD *File_head;
    DIR *Dir_head;
} DIR;

/**
 * @brief 用户类
 */
class CSysUse
{
    DIR *now;
    char code[10];
    char name[10];
    int space;
    int status;
    void set_fname(UFD *&);
    void set_flock(UFD *&);
    void set_fattribute(UFD *&);

public:
    CSysUse();
    ~CSysUse();
    void set_status(int);
    int dele_user();
    int dis_file_metadata();
    int modify_file_metadata();
    int dis_dir(DIR *); // 显示绝对路径
    int get_space();
    char const *get_name();
    char const *get_code();
    int get_status();
    int set_user(char *, char *); // set name and pass
    DIR *get_now();
    int dele_file(UFD *);
    int dele_dir(DIR *);
    int dir_move_up();
    int dis_now(); // dirname
    int new_file();
    int new_dir();
    int open_dir();
    int open_file();
    int edit_file();
    int pre_dele_file();
    int pre_dele_dir();
    int set_code();
};

/**
 * @brief 用户管理系统类
 */
class Cdisk
{
public:
    CSysUse user[5];
    char loginUserName[10];
    char code[10];
    UFD *RootFhead;
    DIR *RootDhead;
    int dis_disk();
    int first_dele_user();
    int dele_user(int);
    int new_user();
    int set_code();
    int login(int);
    Cdisk();
    virtual ~Cdisk();
};

Cdisk DM; // 用户管理实例

/**
 * @description: 清屏函数
 * @param {*}
 * @return {*}
 */
void clear_screen()
{
#ifdef linux
    system("clear");
#elif _UNIX
    system("clear");
#elif _WIN32
    system("cls");
#endif
    cout << flush;
}

enum opt_code
{
    opt_0,
    opt_1,
    opt_2,
    opt_3,
    opt_4,
    opt_5,
    opt_6,
    opt_7,
    opt_8,
    opt_9,
    opt_10,
    opt_11,
    opt_12,
    opt_other
};

opt_code hashit(string const &inString)
{
    if (inString == "0")
        return opt_0;
    if (inString == "1")
        return opt_1;
    if (inString == "2")
        return opt_2;
    if (inString == "3")
        return opt_3;
    if (inString == "4")
        return opt_4;
    if (inString == "5")
        return opt_5;
    if (inString == "6")
        return opt_6;
    if (inString == "7")
        return opt_7;
    if (inString == "8")
        return opt_8;
    if (inString == "9")
        return opt_9;
    if (inString == "10")
        return opt_10;
    if (inString == "11")
        return opt_11;
    if (inString == "12")
        return opt_12;
    return opt_other;
}

void _kbhit(void)
{
    cout << "\n"
         << "输入Y/y继续...";
    string key;
    cin >> key;
    while (key != "Y" && key != "y")
    {
        cout << "输入Y/y继续...";
        cin >> key;
    }
}

void press_key_continue()
{
    _kbhit();
}

/**
 * @description: sleep函数
 * @param {time} 单位毫秒
 * @return {*}
 */
void time_sleep(float time)
{
#ifdef linux
    sleep(time / 1000); // linux sleep 单位为秒，需要除1000
#elif _UNIX
    sleep(time / 1000);
#elif _WIN32
    Sleep(time);
#endif
}

int disk_block[10000];
int disk_empty;
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
Cdisk::Cdisk()
{
    int i = 0;
    strcpy(code, "root");
    for (i = 0; i < 10000; i++) // init 
        disk_block[i] = 0;
    disk_empty = 10000;
    cout.setf(ios::left); // 对齐
    RootDhead = 0;
    RootFhead = 0;
}

/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
Cdisk::~Cdisk()
{
}

/**
 * @description: 删除用户
 * @param {int}
 * @return {1}
 */
int Cdisk::dele_user(int i)
{
    CSysUse C;
    C = user[i];
    user[i].dele_user();
    return 1;
}

/**
 * @description: 展示磁盘信息
 * @param {*}
 * @return {*}
 */
int Cdisk::dis_disk()
{
    int i = 0;
    cout << setw(17) << "用户名" << setw(14) << "占用空间大小" << endl;
    for (i = 0; i < 5; i++)
        if (user[i].get_status() == 1) // 用户存在
            cout << setw(14) << user[i].get_name() << setw(14) << user[i].get_space() << endl;
    cout << endl
         << "已用空间：" << 10000 - disk_empty << endl
         << "剩余空间：" << disk_empty << endl;
    return 1;
}

/**
 * @description: 登陆
 * @param {int}
 * @return {int} 1:success -1:error
 */
int Cdisk::login(int gid)
{
    char n[10], c[10];
    int i;
    if (gid == 0)
    {
        cout << "用户：管理员" << endl;
        cout << "密码：";
        cin >> c;
        while (strcmp(c, code))
        {
            cout << "密码错误，请重新输入" << endl;
            cout << "密码：";
            cin >> c;
        }
        strcpy(DM.loginUserName, "管理员");
        cout << "登陆成功！";
        return 1;
    }
    else
    {
        if (!user[0].get_status()) // 当前不存在用户
        {
            i = 0;
            cout << "当前用户为空，欢迎注册！" << endl;
            user[i].set_status(1);
            cout << "请输入用户名：";
            cin >> n;
            cout << "请输入密码：";
            cin >> c;
            user[i].set_user(n, c);
            strcpy(DM.loginUserName, n);
            cout << "恭喜，创建用户成功！进入系统中..." << endl;
            return i;
        }
        else
        {
            cout << "用户名:";
            cin >> n;
            cout << "密码：";
            cin >> c;
            cout << endl;
            for (i = 0; i < 5; i++) // 匹配用户
            {
                if (user[i].get_status())
                    if (!strcmp(n, user[i].get_name()))
                        if (!strcmp(c, user[i].get_code()))
                        {
                            cout << "登陆成功！" << endl;
                            strcpy(DM.loginUserName, n);
                            cout << "欢迎" << user[i].get_name() << "登陆" << endl;
                            return i;
                        }
                        else
                        {
                            cout << "密码错误" << endl;
                            return -1;
                        }
            }
            cout << "此用户不存在！" << endl;
            return -1;
        }
    }
}

/**
 * @description: 设置新密码
 * @param {*}
 * @return {int} 1:success 0:error
 */
int Cdisk::set_code()
{
    char temp1[10], temp2[10];
    cout << "请输入原密码：";
    cin >> temp1;
    if (strcmp(temp1, code))
    {
        cout << "原密码错误！" << endl;
        return 0;
    }
    while (1)
    {
        cout << "请输入新密码：";
        cin >> temp1;
        cout << "请再次输入新密码：";
        cin >> temp2;
        if (strcmp(temp1, temp2))
        {
            cout << "两次密码不相同，请重输!" << endl;
            break;
        }
        cout << "密码设置成功！" << endl;
        strcpy(code, temp1); // 保存新密码
        break;
    }
    return 1;
}

/**
 * @description: 准备创建新用户
 * @param {*}
 * @return {int} 1:success 0:error
 */
int Cdisk::new_user()
{
    char n[10], c[10];
    int i = 0, j;
    for (i = 0; i < 5; i++)
        if (user[i].get_status() == 0)
            break;
    if (i == 5)
    {
        cout << "已经达到最大用户5个，不能再创建！" << endl;
        return 0;
    }
    user[i].set_status(1);
    cout << "请输入用户名：";
    cin >> n;
    if (i > 0) // 防止冲突
    {
        for (j = 0; j < 5; j++)
        {
            if (j == i || user[i].get_status() == 0)
                continue;
            if (!strcmp(user[j].get_name(), n))
            {
                cout << "此用户名已存在！" << endl;
                return 0;
            }
        }
    }
    cout << "请输入密码：";
    cin >> c;
    user[i].set_user(n, c);

    cout << "恭喜，创建用户成功！" << endl;
    return 1;
}

/**
 * @description: 删除用户
 * @param {*}
 * @return {int} 1:success 0:error
 */
int Cdisk::first_dele_user()
{
    char n[10];
    string confirm;
    int i;
    cout << "请输入你要删除的用户名：";
    cin >> n;
    for (i = 0; i < 5; i++)
        if (!strcmp(user[i].get_name(), n) && user[i].get_status())
            break;
    if (i == 5)
    {
        cout << "出错啦，此用户不存在！" << endl;
        return 0;
    }
    cout << "确认删除此用户？确认Y/y，取消任意键：";
    cin >> confirm;
    if (confirm != "Y" && confirm != "y")
    {
        cout << "已经取消删除！" << endl;
        return 0;
    }
    this->dele_user(i);
    cout << "用户删除成功" << endl;
    return 1;
}

/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
CSysUse::CSysUse()
{
    status = 0;
    space = 0;
    now = 0;
}

/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
CSysUse::~CSysUse()
{
}

/**
 * @description:  创建文件
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::new_file()
{
    int i = 0, j = 0;
    UFD *f, *p = 0;
    DIR *D;

    p = new UFD;
    strcpy(p->lock, ""); // 默认不锁定文件
    p->content = "";     // 内容初始化为空
    if (p == 0)
    {
        cout << "无可用内存空间，创建文件失败！" << endl;
        return 1;
    }
    cout << "请输入建立的文件名：";
    cin >> p->name;
    if (now == 0)
        f = DM.RootFhead;
    else // 其它目录
        f = now->File_head;
    while (f != 0) // 重名冲突检测
    {
        if (!strcmp(p->name, f->name))
        {
            cout << "同名文件已存在！" << endl;
            return 0;
        }
        f = f->next;
    }
    cout << "\n"
         << "空间：";
    cin >> p->space;
    cout << "\n"
         << "属性(0：只读，1：读写)：";
    cin >> p->attribute;
    cout << endl;
    if (p->space > disk_empty) // 空间不足
    {
        cout << "文件太大，空间不足，当前空间为：" << disk_empty << endl;
        delete p;
        return 0;
    }
    disk_empty = disk_empty - p->space;
    // p0 p1 p2 映射到disk_block上
    for (i = 0; i < p->space && i < 10; i++)
    {
        for (j; j < 10000; j++)
        {
            if (disk_block[j] == 0)
            {
                p->p0[i] = j;
                disk_block[j] = 1; // flag
                j++;
                break;
            }
        }
    }
    p->p1 = 0;   
    p->p2 = 0;
    if (p->space > 10)
    {
        p->p1 = new int[100];
        for (i = 10; i < p->space && i < 110; i++)
        {
            for (j; j < 10000; j++)
            {
                if (disk_block[j] == 0)
                {
                    (p->p1)[i - 10] = j;
                    disk_block[j] = 1;
                    j++;
                    break;
                }
            }
        }
        if (p->space > 110)
        {
            p->p2 = new int[100][100];
            for (i = 110; i < p->space; i++)
            {
                for (j; j < 10000; j++)
                {
                    if (disk_block[j] == 0)
                    {
                        int m = (i - 110) / 100; // cow
                        int k = (i - 110) % 100; // col
                        p->p2[m][k] = j;
                        disk_block[j] = 1; // flag
                        j++;
                        break;
                    }
                }
            }
        }
    }
    if (now == 0)
    {
        p->next = DM.RootFhead; // 前插法
        DM.RootFhead = p;
    }
    else
    {
        p->next = now->File_head;
        now->File_head = p;
    }
    space += p->space;
    if (now != 0)
    {
        now->space += p->space;
        D = now->pre;
        while (D != 0) // 递归修改
        {
            D->space += p->space;
            D = D->pre;
        }
    }

    return 0;
}

/**
 * @description: 创建目录
 * @param {*}
 * @return {*}
 */
int CSysUse::new_dir()
{
    DIR *p, *h;
    cout << "请输入新目录的名字：";
    p = new DIR;
    cin >> p->name;
    p->Dir_head = 0;
    p->space = 0;
    p->File_head = 0;
    if (now == 0)
        h = DM.RootDhead;
    else
        h = now->Dir_head;
    while (h != 0)
    {
        if (!strcmp(h->name, p->name))
        {
            cout << "此目录已存在！" << endl;
            return 0;
        }
        h = h->next;
    }
    if (now == 0)
    {
        p->pre = 0;  
        p->next = DM.RootDhead; // 前插法
        DM.RootDhead = p;
    }
    else
    {
        p->pre = now;
        p->next = now->Dir_head;
        now->Dir_head = p;
    }
    cout << "目录创建成功" << endl;
    return 1;
}

/**
 * @description: 返回上级目录
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::dir_move_up()
{
    if (now == 0)
    {
        cout << "已是根目录，不能向上！" << endl;
        return 0;
    }
    now = now->pre;
    return 1;
}

/**
 * @description: 打开目录
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::open_dir()
{
    char name[10];
    DIR *p;
    if (now == 0)
        p = DM.RootDhead;
    else
        p = now->Dir_head;
    cout << "请输入你要打开的目录名：";
    cin >> name;

    while (p != 0)
    {
        if (strcmp(p->name, name) == 0)
        {
            now = p; // match，标记now
            return 1;
        }
        p = p->next;
    }
    cout << "当前目录下不存在该目录" << endl;
    return 0;
}

/**
 * @description: 删除文件前的必要流程
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::pre_dele_file()
{
    char temp[10];
    string confirm;
    cout << "你要删除的文件名：";
    cin >> temp;
    UFD *f = DM.RootFhead;
    UFD *above = 0;
    if (now != 0)
        f = now->File_head;
    while (f != 0)
    {
        if (!strcmp(f->name, temp))
            break;
        above = f; // 标记前驱
        f = f->next;
    }
    if (f == 0)
    {
        cout << "此文件不存在" << endl;
        return 0;
    }
    cout << "确定删除" << f->name << "文件吗？按Y/y确定，其他键取消：";
    cin >> confirm;
    if (confirm != "Y" && confirm != "y")
    {
        cout << "已取消删除文件。" << endl;
        return 0;
    }
    disk_empty += f->space; // 回收总空间
    if (now == 0)
    {
        if (f == DM.RootFhead)
            DM.RootFhead = DM.RootFhead->next;
        else
            above->next = f->next;
    }
    else
    {
        DIR *d = now;
        while (d != 0)
        { // 递归修改目录的占用空间
            d->space -= f->space;
            d = d->pre;
        }
        if (f == now->File_head)
            now->File_head = now->File_head->next;
        else
            above->next = f->next;
    }
    space -= f->space;
    this->dele_file(f);
    cout << "删除成功" << endl;
    return 1;
}

/**
 * @description: 删除文件具体操作
 * @param {*}
 * @return {1}
 */
int CSysUse::dele_file(UFD *f)
{
    int i = 0, m;
    for (i = 0; i < 10 && i < f->space; i++)
    {
        m = f->p0[i];
        disk_block[m] = 0; // reflag to 0
    }
    if (f->p1 != 0)
    {
        for (i = 10; i < 110 && i < f->space; i++)
        {
            m = f->p1[i - 10];
            disk_block[m] = 0;
        }
        delete[](f->p1);
    }
    if (f->p2 != 0)
    {
        for (i = 110; i < f->space; i++)
        {
            m = (f->p2)[(i - 110) / 100][(i - 110) % 100];
            disk_block[m] = 0;
        }
        delete[](f->p2);
        delete f;
    }
    delete f; // 回收
    f = 0;
    return 1;
}

/**
 * @description: 删除目录前的必要流程
 * @param {*}
 * @return {1}
 */
int CSysUse::pre_dele_dir()
{
    char n[10];
    string confirm;
    DIR *p, *above = 0;
    p = DM.RootDhead;
    if (now != 0)
        p = now->Dir_head;
    cout << "要删除的目录名：";
    cin >> n;
    while (p != 0)
    {
        if (!strcmp(p->name, n))
            break;
        above = p; // 记录前驱
        p = p->next;
    }
    if (p == 0)
    {
        cout << "没有这个目录！" << endl;
        return 0;
    }
    cout << "确定删除当前目录及此目标上的所有信息吗？按Y/y确定，其他键取消：";
    cin >> confirm;
    if (confirm != "Y" && confirm != "y")
        return 0;
    disk_empty += p->space; // 回收总空间
    if (now == 0)
    {
        if (p == DM.RootDhead)
            DM.RootDhead = DM.RootDhead->next;
        else
            above->next = p->next;
    }
    else
    {
        if (p == now->Dir_head)
            now->Dir_head = now->Dir_head->next;
        else
            above->next = p->next;
        above = now;
        while (above != 0)
        { // 递归修改占用空间
            above->space -= p->space;
            above = above->pre;
        }
    }
    space -= p->space;
    this->dele_dir(p);
    p = 0;
    cout << "删除成功！" << endl;
    return 1;
}

/**
 * @description: 删除目录具体操作（递归删除 rm -r）
 * @param {DIR *}
 * @return {1}
 */
int CSysUse::dele_dir(DIR *p)
{
    int flag = 0;
    DIR *d = p->Dir_head, *q;
    UFD *f = p->File_head, *r;
    if (f != 0)
    {
        while (f->next != 0)
        {
            r = f->next;
            this->dele_file(f);
            f = r;
            if (f->next == 0)
                this->dele_file(f);
        }
    }
    if (d != 0) // mulu
    {
        while (d->next != 0)
        {
            q = d->next;
            this->dele_dir(d);
            d = q;
            if (d->next == 0)
                this->dele_dir(d);
        }
    }
    delete p;
    p = 0;
    return 1;
}

/**
 * @description: 展示当前目录下文件和目录（dir）
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::dis_now()
{
    DIR *d = DM.RootDhead;
    UFD *f = DM.RootFhead;
    if (now != 0)
    {
        d = now->Dir_head;
        f = now->File_head;
    }
    dis_dir(now);
    cout << endl;
    if (d == 0 && f == 0)
    {
        cout << "当前目录为空" << endl;
        return 0;
    }
    cout << "当前目录大小：";
    if (now == 0)
        cout << space;
    else
        cout << now->space;
    cout << endl;
    if (d == 0)
        cout << "当前目录下没有目录" << endl;
    else
    {
        cout << "当前目录下包含如下目录：" << endl;
        cout << setw(18) << "目录名称" << setw(14) << "目录大小" << endl;
        while (d != 0)
        {
            cout << setw(14) << d->name << setw(14) << d->space << endl;
            d = d->next;
        }
    }
    if (f == 0)
        cout << "当前目录下没有文件" << endl;
    else
    {
        cout << "当前目录下包含如下文件:" << endl;
        cout << setw(18) << "文件名称" << setw(18) << "文件大小" << setw(18) << "文件属性" << setw(18) << "锁定用户" << endl;
        while (f != 0)
        {
            cout << setw(14) << f->name << setw(14) << f->space << setw(14) << f->attribute << setw(14) << f->lock << endl;
            f = f->next;
        }
    }
    return 1;
}

/**
 * @description: 打开文件
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::open_file()
{
    char n[10];
    cout << "请输入要打开的文件名：";
    cin >> n;
    UFD *f = DM.RootFhead;
    if (now != 0)
        f = now->File_head;
    while (f != 0)
    {
        if (!strcmp(f->name, n))
        {
            cout << "文件打开成功，文件内容为" << endl;
            cout << f->content << endl; // show file content
            return 1;
        }
        f = f->next;
    }
    cout << "当前目录无此文件" << endl;
    return 0;
}

/**
 * @description: 编辑文件
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::edit_file()
{
    char n[10];
    string confirm;
    cout << "请输入要编辑的文件名：";
    cin >> n;
    UFD *f = DM.RootFhead;
    if (now != 0)
        f = now->File_head;
    while (f != 0)
    {
        if (!strcmp(f->name, n))
        {
            if (f->attribute == 0)
            {
                cout << "文件为只读，无法编辑，退出！" << endl;
                return 0;
            }
            if (strcmp(f->lock, "")) // 锁定检测
            {
                if (strcmp(f->lock, DM.loginUserName))
                {
                    cout << endl;
                    cout << "无法修改文件元信息! 因为文件" << f->name << "已被用户" << f->lock << "锁定!" << endl;
                    cout << endl;
                    press_key_continue();
                    return 0;
                }
            }
            strcpy(f->lock, DM.loginUserName);
            cout << "文件成功打开，已为用户锁定，文件原内容为" << endl;
            cout << f->content << endl; // show file content
            cout << "是否修改文件内容？确认Y/y，取消任意键：";
            cin >> confirm;
            if (confirm != "Y" && confirm != "y")
            {
                cout << "退出！" << endl;
                return 1;
            }
            while (confirm == "Y" || confirm == "y")
            {
                cout << "修改内容为（最后一行输入EOF结束输入）：";
                string str = "", strline = "";
                getline(cin, strline);
                while (strline != "EOF")
                {
                    str += strline;
                    getline(cin, strline);
                }
                f->content = str;
                cout << "文件已修改，是否继续修改文件内容？确认Y/y，取消任意键：";
                cin >> confirm;
                if (confirm != "Y" && confirm != "y")
                {
                    cout << "退出！" << endl;
                    break;
                }
            }
            return 1;
        }
        f = f->next;
    }
    cout << "当前目录无此文件" << endl;
    return 0;
}

/**
 * @description: 修改管理员密码
 * @param {*}
 * @return {int} 0:error 1:success
 */
int CSysUse::set_code()
{
    char a1[10], a2[10];
    cout << "请输入原密码：";
    cin >> a1;
    if (strcmp(a1, code))
    {
        cout << "密码错误" << endl;
        return 0;
    }
    while (1)
    {
        cout << "请输入新密码：";
        cin >> a1;
        cout << "再次输入新密码:";
        cin >> a2;
        if (strcmp(a1, a2))
            cout << "两次输入密码不同，请重输入！" << endl;
        else
        {
            strcpy(code, a1);
            cout << "密码修改成功！" << endl;
            break;
        }
    }
    return 1;
}

/**
 * @description: 获取当前目录
 * @param {*}
 * @return {DIR *} 指向当前目录的指针
 */
DIR *CSysUse::get_now()
{
    return now;
}

/**
 * @description: 设置用户与密码，创建用户时会调用
 * @param {*}
 * @return {*}
 */
int CSysUse::set_user(char *n, char *c)
{
    strcpy(name, n);
    strcpy(code, c);
    status = 1;
    return 1;
}

/**
 * @description: 标记分配
 * @param {*}
 * @return {*}
 */
void CSysUse::set_status(int b)
{
    status = b;
}

/**
 * @description: 获取分配情况
 * @param {*}
 * @return {*}
 */
int CSysUse::get_status()
{
    return status;
}

/**
 * @description: 返回密码
 * @param {*}
 * @return {*}
 */
const char *CSysUse::get_code()
{
    return code;
}

/**
 * @description: 返回名字
 * @param {*}
 * @return {*}
 */
const char *CSysUse::get_name()
{
    return name;
}

/**
 * @description: 返回空间大小
 * @param {*}
 * @return {*}
 */
int CSysUse::get_space()
{
    return space;
}

/**
 * @description: 返回目录
 * @param {*}
 * @return {0}
 */
int CSysUse::dis_dir(DIR *d)
{
    if (d == 0)
    {
        cout << "/";
        return 0;
    }
    if (d->pre != 0)
        this->dis_dir(d->pre);
    cout << "/" << d->name;
    return 0;
}

/**
 * @description: 修改文件名
 * @param {*}
 * @return {*}
 */
void CSysUse::set_fname(UFD *&f)
{
    cout << "输入新文件名：";
    char n[10];
    cin >> n;
    UFD *fh = DM.RootFhead;
    if (now != 0)
        fh = now->File_head;
    while (fh != 0)
    {
        if (!strcmp(n, fh->name))
            break;
        fh = fh->next;
    }
    if (fh == f || fh == 0)
    {
        strcpy(f->name, n);
        cout << "修改成功" << endl;
    }
    else
    {
        cout << "存在同名文件，退出" << endl;
    }
}

/**
 * @description: 修改锁定属性
 * @param {*}
 * @return {*}
 */
void CSysUse::set_flock(UFD *&f)
{
    string opt;
    if (!strcmp(f->lock, ""))
    {
        cout << "文件未被锁定，是否锁定？确定锁定Y/y，其他键取消：";
        cin >> opt;
        if (opt == "Y" || opt == "y")
        {
            strcpy(f->lock, DM.loginUserName);
            cout << "已锁定";
        }
        else
        {
            cout << "已取消";
        }
    }
    else if (!strcmp(f->lock, DM.loginUserName))
    {
        cout << "文件已被你锁定，是否取消锁定？确定锁定Y/y，其他键取消：";
        cin >> opt;
        if (opt == "Y" || opt == "y")
        {
            strcpy(f->lock, "");
            cout << "已取消锁定";
        }
        else
        {
            cout << "已取消";
        }
    }
    cout << endl;
}

/**
 * @description: 修改读写属性
 * @param {*}
 * @return {*}
 */
void CSysUse::set_fattribute(UFD *&f)
{
    string opt;
    if (f->attribute) // rw
    {
        cout << "文件为读写，是否修改为只读？确定锁定Y/y，其他键取消：";
        cin >> opt;
        if (opt == "Y" || opt == "y")
        {
            f->attribute = 0;
            cout << "已修改为只读";
        }
        else
        {
            cout << "已取消";
        }
    }
    else // r
    {
        cout << "文件为只读，是否修改为读写？确定锁定Y/y，其他键取消：";
        cin >> opt;
        if (opt == "Y" || opt == "y")
        {
            f->attribute = 1;
            cout << "已取消锁定";
        }
        else
        {
            cout << "已取消";
        }
    }
    cout << endl;
}

/**
 * @description: 查看文件元信息
 * @param {*}
 * @return {*}
 */
int CSysUse::dis_file_metadata()
{
    int i;
    char n[10];
    UFD *f = DM.RootFhead;
    if (now != 0)
        f = now->File_head;
    cout << "请输入要查看的文件名：";
    cin >> n;
    while (f != 0)
    {
        if (!strcmp(n, f->name))
            break;
        f = f->next;
    }
    if (f == 0)
    {
        cout << "当前目录下没此文件" << endl;
        return 0;
    }
    if (f->attribute == 0)
        cout << "此为只读文件，";
    else
        cout << "此为读写文件，";
    if (strcmp(f->lock, ""))
    {
        cout << "此文件已被" << f->lock << "锁定，";
        if (strcmp(f->lock, DM.loginUserName))
        {
            cout << endl;
            cout << "无法读取文件，因为文件" << f->name << "已被用户" << f->lock << "锁定!" << endl;
            cout << endl;
            return 0;
        }
    }

    cout << "占用硬盘块号如下：" << endl;
    for (i = 0; i < f->space && i < 10; i++)
    {
        cout << setw(6) << f->p0[i];
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    for (i = 10; i < f->space && i < 110; i++)
    {
        cout << setw(6) << f->p1[i - 10];
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    for (i = 110; i < f->space; i++)
    {
        cout << setw(6) << f->p2[(i - 110) / 100][(i - 110) % 100];
        if ((i + 1) % 10 == 0)
            cout << endl;
    }
    cout << endl;
    return 1;
}

/**
 * @description: 修改文件元信息
 * @param {*}
 * @return {*}
 */
int CSysUse::modify_file_metadata()
{
    int i;
    char n[10];
    UFD *f = DM.RootFhead;
    if (now != 0)
        f = now->File_head;
    cout << "请输入要修改的文件名：";
    cin >> n;
    while (f != 0)
    {
        if (!strcmp(n, f->name))
            break;
        f = f->next;
    }
    if (f == 0)
    {
        cout << "当前目录下没此文件" << endl;
        return 0;
    }
    bool flag = true;
    while (flag)
    {
        cout << "文件名" << f->name << ",";
        if (f->attribute == 0)
            cout << "此为只读文件，";
        else
            cout << "此为读写文件，";
        if (strcmp(f->lock, ""))
        {
            cout << "此文件已被" << f->lock << "锁定" << endl;
            if (strcmp(f->lock, DM.loginUserName))
            {
                cout << endl;
                cout << "无法修改文件元信息! 因为文件" << f->name << "已被用户" << f->lock << "锁定!" << endl;
                cout << endl;
                // press_key_continue();
                return 0;
            }
        }
        else
            cout << "此文件未被锁定" << endl;
        string opt;
        cout << "╭----------------------------------------------------------------╮" << endl;
        cout << "|                                                                |" << endl;
        cout << "|                        1.修改文件名                            |" << endl;
        cout << "|                        2.修改读写属性                          |" << endl;
        cout << "|                        3.修改锁定属性                          |" << endl;
        cout << "|                        0.退出                                  |" << endl;
        cout << "|                                                                |" << endl;
        cout << "╰----------------------------------------------------------------╯" << endl;
        cout << endl;
        cout << "请输入选择：";
        cin >> opt;
        switch (hashit(opt))
        {
        case opt_1:
            set_fname(f);
            break;
        case opt_2:
            set_fattribute(f);
            break;
        case opt_3:
            set_flock(f);
            break;
        case opt_0:
            flag = false;
            break;
        default:
            cout << "请输入0-3" << endl;
            break;
        }
        if (opt != "0")
        {
            press_key_continue();
            clear_screen();
        }
    }
    return 1;
}

/**
 * @description: 删除用户
 * @param {*}
 * @return {1}
 */
int CSysUse::dele_user()
{
    space = 0;
    now = 0;
    status = 0;
    return 1;
}

int main()
{
    string opt;
    int uid, flag = 1;

    while (flag)
    {
        cout << "╭------------------------欢迎使用文件系统------------------------╮" << endl;
        cout << "|                                                                |" << endl;
        cout << "|                        1.管理员登陆                            |" << endl;
        cout << "|                        2.用户登陆                              |" << endl;
        cout << "|                        0.退出系统                              |" << endl;
        cout << "|                                                                |" << endl;
        cout << "╰----------------------------------------------------------------╯" << endl;

        cout << "\n请输入选择：";
        cin >> opt;
        switch (hashit(opt))
        {
        case opt_1:
            uid = DM.login(0);
            flag = 1;
            cout << "管理员登陆成功！" << endl;
            time_sleep(1000);
            while (flag)
            {
                clear_screen();
                cout << "╭----------------------------------------------------------------╮" << endl;
                cout << "|                                                                |" << endl;
                cout << "|                          1.创建用户                            |" << endl;
                cout << "|                          2.删除用户                            |" << endl;
                cout << "|                          3.查看现存用户                        |" << endl;
                cout << "|                          4.修改密码                            |" << endl;
                cout << "|                          0.返回登陆窗口                        |" << endl;
                cout << "|                                                                |" << endl;
                cout << "╰----------------------------------------------------------------╯" << endl;
                cout << "请选择：";
                cin >> opt;
                clear_screen();
                switch (hashit(opt))
                {
                case opt_1:
                    DM.new_user();
                    break;
                case opt_2:
                    DM.first_dele_user();
                    break;
                case opt_3:
                    DM.dis_disk();
                    break;
                case opt_4:
                {
                    DM.set_code();
                    break;
                }
                case opt_0:
                {
                    flag = 0;
                    clear_screen();
                    break;
                }
                default:
                    cout << "请输入0-4！" << endl;
                }
                if (opt != "0") // 按0退出便直接退出
                    press_key_continue();
            }
            flag = 1;
            break;
        case opt_2:
            uid = DM.login(1000);
            if (uid == -1)
                break;
            time_sleep(1500);
            while (flag)
            {
                clear_screen();
                cout << "╭----------------------------------------------------------------╮" << endl;
                cout << "|                                                                |" << endl;
                cout << "|                          1.创建文件                            |" << endl;
                cout << "|                          2.删除文件                            |" << endl;
                cout << "|                          3.创建目录                            |" << endl;
                cout << "|                          4.删除目录                            |" << endl;
                cout << "|                          5.打开目录                            |" << endl;
                cout << "|                          6.返回上一层目录                      |" << endl;
                cout << "|                          7.查看当前目录                        |" << endl;
                cout << "|                          8.修改密码                            |" << endl;
                cout << "|                          9.查看文件元信息                      |" << endl;
                cout << "|                         10.编辑文件元信息                      |" << endl;
                cout << "|                         11.打开文件                            |" << endl;
                cout << "|                         12.编辑文件                            |" << endl;
                cout << "|                          0.安全退出                            |" << endl;
                cout << "|                                                                |" << endl;
                cout << "╰----------------------------------------------------------------╯" << endl;
                cout << "用户：" << DM.user[uid].get_name() << '\n'
                     << "目录为：";
                DM.user[uid].dis_dir(DM.user[uid].get_now());
                cout << endl;
                cout << "请输入选择：";
                cin >> opt;
                clear_screen();
                switch (hashit(opt))
                {
                case opt_1:
                    DM.user[uid].new_file();
                    break;
                case opt_2:
                    DM.user[uid].pre_dele_file();
                    break;
                case opt_3:
                    DM.user[uid].new_dir();
                    break;
                case opt_4:
                    DM.user[uid].pre_dele_dir();
                    break;
                case opt_5:
                    DM.user[uid].open_dir();
                    break;
                case opt_6:
                    DM.user[uid].dir_move_up();
                    break;
                case opt_7:
                    DM.user[uid].dis_now();
                    break;
                case opt_8:
                    DM.user[uid].set_code();
                    break;
                case opt_9:
                    DM.user[uid].dis_file_metadata();
                    break;
                case opt_10:
                    DM.user[uid].modify_file_metadata();
                    break;
                case opt_11:
                    DM.user[uid].open_file();
                    break;
                case opt_12:
                    DM.user[uid].edit_file();
                    break;
                case opt_0:
                {
                    flag = 0;
                    clear_screen();
                    break;
                }
                default:
                    cout << "请输入0-9" << endl;
                }
                if (opt != "0")
                    press_key_continue();
            }
            flag = 1;
            break;
        case opt_0:
            flag = 0;
            break;
        default:
            cout << "请输入0-2！" << endl;
            time_sleep(2000);
            clear_screen();
        }
        if (opt != "0")
        { // 按0退出便直接退出
            press_key_continue();
            clear_screen();
        }
    }
    return 0;
}
