#include "rsa.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <thread>
#include <windows.h>
#include <string>

RSA::RSA()
{
	// initKeys();
}

void RSA::createKey()
{
	initKeys();

	std::ofstream fout1("ekey.txt");
	fout1 << _key._ekey << "\n";
	fout1 << _key._pkey << "\n";

	std::ofstream fout2("dkey.txt");

	fout2 << _key._dkey << "\n";
	fout2 << _key._pkey << "\n";

	fout1.close();
	fout2.close();
}

void RSA::ecrept(const char* fileekey, const char* filein, const char* fileout)
{
	std::ifstream fkey(fileekey);
	std::ifstream fin(filein, std::ifstream::binary);
	std::ofstream fout(fileout, std::ofstream::binary);
	if (!fin.is_open())
	{
		perror("ecrept: input file open failed!");
		system("pause");
		return;
	}

	std::string str;
	std::getline(fkey, str);
	DataType temp1(str);
	std::getline(fkey, str);
	DataType temp2(str);
	_key._ekey = temp1;
	_key._pkey = temp2;

	char* bufferIn = new char[BUFFER_SIZE];
	DataType* bufferOut = new DataType[BUFFER_SIZE];

	while (!fin.eof())
	{
		fin.read(bufferIn, BUFFER_SIZE);
		int curNum = fin.gcount(); // �������һ�ζ�ȡ���ֽ���
		for (int i = 0; i < curNum; i++)
		{
			bufferOut[i] = ecrept((DataType)bufferIn[i], _key._ekey, _key._pkey);
		}
		fout.write((char*)bufferOut, curNum * sizeof(DataType));
	}

	fin.close();
	fout.close();
	delete[] bufferIn;
	delete[] bufferOut;
}

void RSA::decrept(const char* fileekey, const char* filein, const char* fileout)
{
	std::ifstream fkey(fileekey);
	std::ifstream fin(filein, std::ifstream::binary);
	std::ofstream fout(fileout, std::ofstream::binary);
	if (!fin.is_open())
	{
		perror("decrept: input file open failed!");
		system("pause");
		return;
	}

	std::string str;
	std::getline(fkey, str);
	DataType temp1(str);
	std::getline(fkey, str);
	DataType temp2(str);
	_key._dkey = temp1;
	_key._pkey = temp2;

	DataType* bufferIn = new DataType[BUFFER_SIZE];
	char* bufferOut = new char[BUFFER_SIZE];

	while (!fin.eof())
	{
		fin.read((char*)bufferIn, BUFFER_SIZE * sizeof(DataType));
		int num = fin.gcount(); // �������һ�ζ�ȡ���ֽ���
		num /= sizeof(DataType);
		for (int i = 0; i < num; i++)
		{
			bufferOut[i] = (char)decrept(bufferIn[i], _key._dkey, _key._pkey);
		}
		fout.write(bufferOut, num);
	}
	fin.close();
	fout.close();
	delete[] bufferIn;
	delete[] bufferOut;
}

void RSA::initKeys()
{
	// DataType prime1 = getPrime();
	// DataType prime2 = getPrime();
	// ������Ҫ����ʱ���Ƕ����������ĳ�ʼ��
	DataType prime1, prime2;
	std::thread t1(&RSA::getPrime, this, std::ref(prime1));
	// getPrime()��ʱ��Ϊ����, ����ͣ����ֹ�߳�������ͬ
	Sleep(1000);
	std::thread t2(&RSA::getPrime, this, std::ref(prime2));
	t1.join();
	t2.join();
	while (prime1 == prime2)
	{
		std::cout << "prime1 == prime2, wait a moment!" << std::endl;
		getPrime(prime1);
	}

	DataType orla = getOrla(prime1, prime2);
	_key._pkey = getPkey(prime1, prime2);
	_key._ekey = getEkey(orla);
	_key._dkey = getDkey(_key._ekey, orla);
}

Key RSA::getKeys()
{
	return _key;
}

DataType RSA::ecrept(DataType data, DataType ekey, DataType pkey)
{
	// ֱ�������ݴη�ʱ�临�Ӷȹ���
	// ʹ�ÿ�����ȡģ�����㷨
	DataType Ai = data;
	DataType msgE = 1;

	while (ekey)
	{
		if (ekey & 1)
		{
			msgE = (msgE * Ai) % pkey;
		}
		ekey >>= 1;
		Ai = (Ai * Ai) % pkey;
	}
	return msgE;
}

DataType RSA::decrept(DataType data, DataType dkey, DataType pkey)
{
	return ecrept(data, dkey, pkey);
}


void RSA::getPrime(DataType& prime)
{
	brdm::mt19937 gen(time(NULL));
	brdm::uniform_int_distribution<DataType> dist(0, DataType(1) << 256);
	while (true)
	{
		prime = dist(gen);
		if (isPrimeBigInt(prime))
		{
			break;
		}
	}
}

bool RSA::isPrime(DataType data)
{
	if (data <= 0)
	{
		return false;
	}
	for (int i = 2; i <= sqrt(data); i++)
	{
		if (data % i == 0)
			return false;
	}
	return true;
}

bool RSA::isPrimeBigInt(DataType data)
{
	brdm::mt11213b gen(time(NULL));
	if (miller_rabin_test(data, 25, gen))
	{
		if (miller_rabin_test((data - 1) / 2, 25, gen))
		{
			return true;
		}
	}
	return false;
}

DataType RSA::getPkey(DataType prime1, DataType prime2)
{
	return prime1 * prime2;
}

DataType RSA::getOrla(DataType prime1, DataType prime2)
{
	return (prime1 - 1) * (prime2 - 1);
}

DataType RSA::getEkey(DataType orla)
{
	brdm::mt11213b gen(time(NULL));
	brdm::uniform_int_distribution<DataType> dist(2, orla);
	DataType ekey;
	while (true)
	{
		ekey = dist(gen);
		if (ekey > 1 && getGcd(ekey, orla) == 1)
		{
			return ekey;
		}
	}
}

DataType RSA::getDkey(DataType ekey, DataType orla)
{
	DataType x = 0, y = 0;
	exGcd(ekey, orla, x, y);
	return (x % orla + orla) % orla;
	/*
	DataType dkey = orla / ekey;
	while (true)
	{
		if ((dkey * ekey) % orla == 1)
		{
			break;
		}
		++dkey;
	}
	return dkey;
	*/
}

DataType RSA::getGcd(DataType data1, DataType data2)
{
	// շת�����
	DataType residual = 0; // ����
	while (residual = data1 % data2)
	{
		data1 = data2;
		data2 = residual;
	}
	return data2;
}

DataType RSA::exGcd(DataType a, DataType b, DataType& x, DataType& y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	DataType gcd = exGcd(b, a % b, x, y);
	DataType x1 = x, y1 = y;
	x = y1;
	y = x1 - a / b * y1;
	return gcd;
}

