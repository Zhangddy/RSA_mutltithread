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
				// ����ѭ��
			}
		}
	}

	int mainMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*       ---RSA ���ܹ��� (256bit)---     *" << endl;
		cout << "*          ��ѡ����Ҫ���еĲ���         *" << endl;
		cout << "*          �� ������Կ�͹�Կ            *" << endl;
		cout << "*          �� ͨ��˽Կ��������          *" << endl;
		cout << "*          �� ͨ����Կ��������          *" << endl;
		cout << "*          �� �˳�                      *" << endl;
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
		cout << "*           �Բ���������������          *" << endl;
		cout << "*              ����������!              *" << endl;
		cout << "*                                       *" << endl;
		cout << "*           ���������������...         *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		system("pause");
	}

	void exitMenu()
	{
		system("cls");
		cout << "*****************************************" << endl;
		cout << "*                                       *" << endl;
		cout << "*            ----�����˳�----           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                 ���Ժ�...             *" << endl;
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
		cout << "*            ----���ڽ���----           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                 ���Ժ�...             *" << endl;
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
		cout << "*            ----�����!----            *" << endl;
		cout << "*                                       *" << endl;
		cout << "*             �����������...           *" << endl;
		cout << "*                                       *" << endl;
		cout << "*                                       *" << endl;
		cout << "*****************************************" << endl;
		system("pause");
	}
private:
	RSA rsa;
};
