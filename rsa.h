#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp> // 素性检测头文件

#define BUFFER_SIZE 256

typedef boost::multiprecision::int1024_t DataType;
namespace brdm = boost::random;

struct Key
{
	// 公钥: (e, n)
	// 私钥: (d, n)
	DataType _ekey; // 加密秘钥
	DataType _dkey; // 解密秘钥
	DataType _pkey; // 公共部分
};

/*
	1. 产生两个素数		随机生成pq
	2. 求n				n = p * q
	3. 求f(n)			f(n) = (p - 1)(q - 1)
	4. 求e				1 < e < f(n), 且 e和f(n) 互质
	5. 求d				e * d % f(n) = 1
	得到: (e, n) (d, n)
*/
class RSA
{
public:
	RSA();

	void createKey();
	void ecrept(const char* fileekey, const char* filein, const char* fileout);
	void decrept(const char* fileekey, const char* filein, const char* fileout);

	DataType ecrept(DataType data, DataType ekey, DataType pkey);	// 加密运算
	DataType decrept(DataType data, DataType dkey, DataType pkey);	// 解密运算
	bool isPrime(DataType data);
	void getPrime(DataType& prime);
	bool isPrimeBigInt(DataType data);
	DataType getPkey(DataType prime1, DataType prime2);
	DataType getOrla(DataType prime1, DataType prime2);
	DataType getEkey(DataType orla);
	DataType getDkey(DataType ekey, DataType orla);
	DataType getGcd(DataType data1, DataType data2); // 返回最大公约数
	DataType exGcd(DataType a, DataType b, DataType& x, DataType& y); // 求模反元素 加密秘钥
	void initKeys();
	Key getKeys();

private:
	Key _key;
};



