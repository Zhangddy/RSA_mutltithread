#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp> // ���Լ��ͷ�ļ�

#define BUFFER_SIZE 256

typedef boost::multiprecision::int1024_t DataType;
namespace brdm = boost::random;

struct Key
{
	// ��Կ: (e, n)
	// ˽Կ: (d, n)
	DataType _ekey; // ������Կ
	DataType _dkey; // ������Կ
	DataType _pkey; // ��������
};

/*
	1. ������������		�������pq
	2. ��n				n = p * q
	3. ��f(n)			f(n) = (p - 1)(q - 1)
	4. ��e				1 < e < f(n), �� e��f(n) ����
	5. ��d				e * d % f(n) = 1
	�õ�: (e, n) (d, n)
*/
class RSA
{
public:
	RSA();

	void createKey();
	void ecrept(const char* fileekey, const char* filein, const char* fileout);
	void decrept(const char* fileekey, const char* filein, const char* fileout);

	DataType ecrept(DataType data, DataType ekey, DataType pkey);	// ��������
	DataType decrept(DataType data, DataType dkey, DataType pkey);	// ��������
	bool isPrime(DataType data);
	void getPrime(DataType& prime);
	bool isPrimeBigInt(DataType data);
	DataType getPkey(DataType prime1, DataType prime2);
	DataType getOrla(DataType prime1, DataType prime2);
	DataType getEkey(DataType orla);
	DataType getDkey(DataType ekey, DataType orla);
	DataType getGcd(DataType data1, DataType data2); // �������Լ��
	DataType exGcd(DataType a, DataType b, DataType& x, DataType& y); // ��ģ��Ԫ�� ������Կ
	void initKeys();
	Key getKeys();

private:
	Key _key;
};


