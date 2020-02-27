#include <iostream>
#include <cstdlib>
#include "rsa.h"
#include <Windows.h>

using std::cout;
using std::endl;
using std::cin;

class Menu
{
public:
	Menu()
		:rsa()
	{

	}

	void mainFunc()
	{
		int option = 0;
		while (1)
		{
			option = mainMenu();
			if (option == 1)
			{
				waitMenu();
				rsa.createKey();
				completeMenu();
			}
			else if (option == 2)
			{
				waitMenu();
				rsa.decrept("dkey.txt","test.ecrept.txt","test.decrept.txt");
				completeMenu();
			}
			else if (option == 3)
			{
				waitMenu();
				rsa.ecrept("ekey.txt", "test.txt", "test.ecrept.txt");
				completeMenu();
			}
			else if (option == 4)
			{
				exitMenu();
				break;
			}
			else
			{
				errorMenu();
				// 继续循环
			}
		}
	}

	int mainMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*       ---RSA 加密工具 (256bit)---     *" << endl;
		cout << "*          请选择你要进行的操作         *" << endl;
		cout << "*          ① 产生秘钥和公钥            *" << endl;
		cout << "*          ② 通过私钥解密密文          *" << endl;
		cout << "*          ③ 通过公钥加密明文          *" << endl;
		cout << "*          ④ 退出                      *" << endl;
		cout << "*****************************************" << endl;
		int option = 0;
		cin >> option;
		return option;
	}

	void errorMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*                                       *" << endl;
		cout << "*           对不起您的输入有误          *" << endl;
		cout << "*              请重新输入!              *" << endl;
		cout << "*                                       *" << endl;
		cout << "*           ◆按任意键继续◆...         *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		system("pause");
	}

	void exitMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*                                       *" << endl;
		cout << "*            ----正在退出----           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                 请稍后...             *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		Sleep(1000);
	}

	void waitMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*                                       *" << endl;
		cout << "*            ----正在进行----           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                 请稍后...             *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		Sleep(1000);
	}

	void completeMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*                                       *" << endl;
		cout << "*            ----已完成!----            *" << endl;
		cout << "*                                       *" << endl;
		cout << "*             按任意键继续...           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		system("pause");
	}
private:
	RSA rsa;
};
