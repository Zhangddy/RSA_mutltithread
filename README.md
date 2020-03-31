
## 项目介绍 · **数据加密工具**

## 项目简介
本项目基于RSA算法实现对数据的非对称加密，RSA算法是通过生成公钥私钥来对数据进行加密，通过用公钥加密私钥解密的方式避免了传输密钥的不安全性。RSA的安全基于大数分解的难度，其公钥和私钥是一对大素数（100到200位十进制数或更大）的函数。从一个公钥和密文恢复出明文的难度，等价于分解两个大素数之积（这是公认的数学难题）并且一直是最广为使用的"非对称加密算法"。主要应用在网络通信协议Https, ssh登录等方面。
## 主要技术
* RSA非对称加密
* 大数运算
* 文件IO
* 多线程

## 项目开发
**1.对称加密和非对称加密**
* **对称加密：**
    
    1976年以前，所有的加密方法都是同一种模式：甲方选择某一种加密规则，对信息进行加密。乙方使用同一种规则，对信息进行解密。由于加密和解密使用同样规则，这被称为"对称加密算法"。这种加密模式有一个最大弱点：甲方必须把加密规则告诉乙方，否则无法解密。保存和传递密钥，就成了最头疼的问题。
* **非对称加密**

    非对称加密算法需要两个密钥：公开密钥和私有密钥。公钥与私钥是一对，如果用公钥对数据进行加密，只有用对应的私钥才能解密。因为加密和解密使用的是两个不同的密钥，所以这种算法叫作非对称加密算法。 非对称加密算法实现机密信息交换的基本过程是：甲方生成一对密钥并将公钥公开，需要向甲方发送信息的其他角色(乙方)使用该密钥(甲方的公钥)对机密信息进行加密后再发送给甲方；甲方再用自己私钥对加密后的信息进行解密。甲方想要回复乙方时正好相反，使用乙方的公钥对数据进行加密，同理，乙方使用自己的私钥来进行解密。
    
---

**2.Boost大数库**   
    
> Boost库是为C++语言标准库提供扩展的一些C++程序库的总称，由Boost社区组织开发、维护。Boost库可以与C++标准库完美共同工作，并且为其提供扩展功能。Multiprecision库：提供比C++内置的整数、分数和浮点数精度更高的多精度数值运算功能。在RSA加密算法中需要产生1024bit位以上的数据，此时自定义类型范围以及不满足，并且RSA算法中会用到产生随机大数和大数的素性检测，自身实现太过复杂，故使用Boost大数库来进行高精度运算。

* 产生大数随机数

    boost库中可以提供产生大数随机数的接口，它在random.hpp文件中

    例如
    ```cpp
    #include <boost/multiprecision/random.hpp>
    namespace rp = boost::random;
    void test()
    {
        //mt19937:一种随机数产生器
        rp::mt19937 gen(time(nullptr));
        cout << "random" << endl;
        //指定随机数的范围 0 ~ (1<<256)
        rp::uniform_int_distribution<mp::cpp_int> dist(0, mp::cpp_int(1)<<256);
        cout << dist(gen) << endl;
    }
    ```

* 大数素性检测


    普通的素数检测方法对于大数的效率太慢，大数的素性检测有专门的算法，比如fermat检测，Miller-Rabin等算法。boost库中实现了Miller-Rabin方法
    
    例如
    ```cpp
    #include <boost/multiprecision/miller_rabin.hpp>
    namespace rp = boost::random;
    Rsa::is_prime_bigInt(const mp::int1024_t digit)
    {

        rp::mt11213b gen(time(nullptr));
        if (miller_rabin_test(digit, 25, gen))
        {
            if (miller_rabin_test((digit - 1) / 2, 25, gen))
            {
                return true;
            }
        }
        return false;
    }
    ```

**3.RSA加密方式**

* RSA加密公式
  * 加密公式：公钥(E, N)
  
    &ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp; 密文 = ($明文^E$) % N
  * 解密公式：私钥(D, N)
    
    &ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;&ensp; 明文 = ($密文^D$) % N
  


* RSA密钥产生过程

![流程图](https://img-blog.csdnimg.cn/20200227163610704.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L25ld19iZWVfMDE=,size_20,color_FF1111,t_30)
* **实例描述**
1. 选择p = 3, q = 11。
2. n = pq = 33。
3. φ(n) = (p - 1)(q - 1) = 20。
4. 选择e = 3, 此时e与φ(n) 互质
5. (de)modφ(n) =(d*3)mod20=1,求出d=7。
6. 公钥(3,33)，私钥(7,33)。
7. 例如对明文数字“5”加密，根据加密公式，密文=(5^3)mod(33) = 125 % 33 = 26
8. 把上述密文解密，明文=26^7 % 33 = 8031810176 % 33 = 5


**4.主要函数及数据结构**
* 数据结构

```cpp
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
```

* 获取两个素数
```cpp
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
```

* 判断素数

```cpp
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
```

* 计算欧拉函数

```cpp
DataType RSA::getOrla(DataType prime1, DataType prime2)
{
	return (prime1 - 1) * (prime2 - 1);
}
```

* 得到密钥

```cpp

DataType RSA::getPkey(DataType prime1, DataType prime2)
{
	return prime1 * prime2;
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
}
```

* 快速模取运算进行加密

```cpp
DataType RSA::ecrept(DataType data, DataType ekey, DataType pkey)
{
	// 直接运算幂次方时间复杂度过大
	// 使用快速幂取模运算算法
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
```

* 模反元素优化

```cpp
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
```

* **多线程初始函数**

```cpp
void RSA::initKeys()
{
	// DataType prime1 = getPrime();
	// DataType prime2 = getPrime();
	// 程序主要运行时间是对两个素数的初始化
	DataType prime1, prime2;
	std::thread t1(&RSA::getPrime, this, std::ref(prime1));
	// getPrime()以时间为种子, 短暂停留防止线程种子相同
	Sleep(100);
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
```

**5.源代码**

[https://github.com/Zhangddy/RSA_mutltithread](https://github.com/Zhangddy/RSA_mutltithread)

