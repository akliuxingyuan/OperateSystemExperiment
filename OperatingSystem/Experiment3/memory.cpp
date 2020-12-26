/*
 * @Author: akliuxingyuan
 * @Description: memory manage
 * @Date: 2020-12-22 12:06:31
 */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct freeArea
{
	int startingAddress;
	int length;
} freeArea;

typedef struct alreadyAssignedArea
{
	int startingAddress;
	int length;
	string remark;
} alreadyAssignedArea;

void initFreeAreaTable(vector<freeArea> &p)
{
	freeArea s;
	int startingAddress[] = {45, 110};
	int length[] = {20, 146};
	for (int i = 0; i < 2; i++)
	{
		s.startingAddress = startingAddress[i];
		s.length = length[i];
		p.push_back(s);
	}
	cout << endl;
}

void printFreeArea(vector<freeArea> p)
{
	cout << "空闲区表如下:" << endl;
	cout << "起始地址\t"
		 << "长度\t"
		 << "状态" << endl;
	for (vector<freeArea>::iterator it = p.begin(); it != p.end(); it++)
	{
		cout << it->startingAddress << "K\t\t" << it->length << "KB\t"
			 << "未分配" << endl;
	}
	cout << endl;
}

void initAlreadyAssignedAreaTable(vector<alreadyAssignedArea> &p)
{
	alreadyAssignedArea s;
	int startingAddress[] = {0, 10, 20, 45, 65, 110};
	int length[] = {10, 10, 25, 20, 45, 146};
	string remark[] = {"操作系统", "作业1", "作业4", "空闲区", "作业2", "空闲区"};
	for (int i = 0; i < 6; i++)
	{
		s.startingAddress = startingAddress[i];
		s.length = length[i];
		s.remark = remark[i];
		p.push_back(s);
	}
	cout << endl;
}

void printAleadyAssignedArea(vector<alreadyAssignedArea> p)
{
	cout << "已分配分区表如下:" << endl;
	cout << "起始地址\t"
		 << "长度\t"
		 << "备注" << endl;
	for (vector<alreadyAssignedArea>::iterator it = p.begin(); it != p.end(); it++)
	{
		cout << it->startingAddress << "K\t\t" << it->length << "KB\t" << it->remark << endl;
	}
	cout << endl;
}

void firstFitAllocation(vector<freeArea> &p, vector<alreadyAssignedArea> &q)
{
	int length;
	string remark;
	bool flag = false;
	string choose;
	cout << "你想要分配吗?,yes or no:";
	cin >> choose;
	while (choose == "yes")
	{
		flag = false;
		cout << "请输入要申请的作业的长度和备注，例如(20 作业1):";
		cin >> length;
		cin >> remark;
		for (vector<freeArea>::iterator it = p.begin(); it != p.end(); it++)
		{
			if (length > it->length)
				continue;
			else if (it->length == length)
			{
				cout << "分配成功" << endl;
				for (vector<alreadyAssignedArea>::iterator it2 = q.begin(); it2 != q.end(); it2++)
				{
					if (it2->startingAddress == it->startingAddress)
					{
						it2->remark = remark;
						break;
					}
				}
				p.erase(it);
				flag = true;
				break;
			}
			else
			{
				cout << "分配成功" << endl;
				alreadyAssignedArea t;
				t.startingAddress = it->startingAddress + length;
				t.length = it->length - length;
				t.remark = "空闲区";
				for (vector<alreadyAssignedArea>::iterator it2 = q.begin(); it2 != q.end(); it2++)
				{
					if (it2->startingAddress == it->startingAddress)
					{
						it2->length = length;
						it2->remark = remark;
						q.insert(++it2, t);
						break;
					}
				}
				it->length = it->length - length;
				it->startingAddress = it->startingAddress + length;
				flag = true;
				break;
			}
		}
		if (!flag)
		{
			cout << "抱歉,没有适合的空间可以分配" << endl;
		}
		else
		{
			printFreeArea(p);
			printAleadyAssignedArea(q);
		}
		cout << "你还想要分配吗?,yes or no:";
		cin >> choose;
	}
}

void firstFitRecovery(vector<freeArea> &p, vector<alreadyAssignedArea> &q)
{
	freeArea s;
	string remark;
	string choose;
	bool flag1, flag2;
	cout << "你想要回收吗?,yes or no:";
	cin >> choose;
	while (choose == "yes")
	{
		cout << "请输入需要回收的内存空间的备注:";
		cin >> remark;
		flag1 = true;
		flag2 = true;
		vector<alreadyAssignedArea>::iterator itprer;
		vector<alreadyAssignedArea>::iterator it = q.begin();
		vector<alreadyAssignedArea>::iterator itnext;
		for (it = q.begin(); it != q.end(); it++)
			if (it->remark == remark)
				break;
		itprer = it;
		itnext = it;
		if (it == q.begin())
		{
			flag1 = false;
			itnext++;
		}
		else if (it == --q.end())
		{
			flag2 = false;
			itprer--;
		}
		else
		{
			itprer--;
			itnext++;
		}
		if (flag1 && flag2 && itprer->remark == "空闲区" && itnext->remark == "空闲区")
		{
			itprer->length += it->length + itnext->length;
			vector<freeArea>::iterator i;
			for (i = p.begin(); i != p.end(); i++)
				if (i->startingAddress == itprer->startingAddress)
					break;
			i->length += it->length + itnext->length;
			q.erase(itnext);
			q.erase(it);
			p.erase(++i);
		}
		else if (flag1 && itprer->remark == "空闲区")
		{
			itprer->length += it->length;
			q.erase(it);
			vector<freeArea>::iterator i;
			for (i = p.begin(); i != p.end(); i++)
				if (i->startingAddress == itprer->startingAddress)
					break;
			i->length = itprer->length;
		}
		else if (flag2 && itnext->remark == "空闲区")
		{
			vector<freeArea>::iterator i;
			for (i = p.begin(); i != p.end(); i++)
				if (i->startingAddress == itnext->startingAddress)
					break;
			itnext->startingAddress = it->startingAddress;
			itnext->length += it->length;
			i->startingAddress = it->startingAddress;
			i->length = itnext->length;
			q.erase(it);
		}
		else
		{
			vector<freeArea>::iterator i;
			for (i = p.begin(); i != p.end(); i++)
				if (i->startingAddress > it->startingAddress)
					break;
			it->remark = "空闲区";
			s.startingAddress = it->startingAddress;
			s.length = it->length;
			p.insert(i, s);
		}
		printFreeArea(p);
		printAleadyAssignedArea(q);
		cout << "你还想要回收吗?,yes or no:";
		cin >> choose;
	}
}

int main()
{
	vector<freeArea> p;
	vector<alreadyAssignedArea> q;
	int choose;
	initFreeAreaTable(p);
	printFreeArea(p);
	initAlreadyAssignedAreaTable(q);
	printAleadyAssignedArea(q);
	while (true)
	{
		cout << "1:分配           2：回收         3:退出" << endl;
		cout << "请输入:";
		cin >> choose;
		if (choose == 1)
			firstFitAllocation(p, q);
		else if (choose == 2)
			firstFitRecovery(p, q);
		else if (choose == 3)
			break;
		else
			cout << "输入有误";
	}
	getchar();
	getchar();
	return 0;
}
