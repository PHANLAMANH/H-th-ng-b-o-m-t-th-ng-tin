#include <iostream>
#include <bitset>
#include <random>
using namespace std;

// generate operator<= for std::bitset<8>
bool operator<=(const std::bitset<8> &a, const std::bitset<8> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return b[i];
    }
    return true;
}

bool Subtracter(bool a, bool b, bool &borrow)
{
    bool diff;
    if (borrow)
    {
        diff = !(a ^ b);
        borrow = !a || (a && b);
    }
    else
    {
        diff = a ^ b;
        borrow = !a && b;
    }
    return diff;
}

bitset<8> subBin(bitset<8> x, bitset<8> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<8> ans;
    for (int i = 0; i < 7; i++)
    {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

// generate operator> for std::bitset<8>
bool operator>(const std::bitset<8> &a, const std::bitset<8> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return false;
}

// generate operator+ for std::bitset<8>
std::bitset<8> operator+(const std::bitset<8> &a, const std::bitset<8> &b)
{
    std::bitset<8> result(0);
    bool carry = false;
    for (int i = 0; i < 8; ++i)
    {
        if (a[i] == 1 && b[i] == 1)
        {
            if (carry)
            {
                result[i] = 1;
            }
            else
            {
                carry = true;
            }
        }
        else if (a[i] == 1 || b[i] == 1)
        {
            if (carry)
            {
                carry = true;
            }
            else
            {
                result[i] = 1;
            }
        }
        else
        {
            if (carry)
            {
                result[i] = 1;
                carry = false;
            }
        }
    }
    return result;
}

// generate operator= for std::bitset<8>
//  std::bitset<8> operator=(const std::bitset<8> &a)
//  {
//      std::bitset<8> result(0);
//      for (int i = 0; i < 8; ++i)
//      {
//          result[i] = a[i];
//      }
//      return result;
//  }

// generate operator>= for std::bitset<8>
bool operator>=(const std::bitset<8> &a, const std::bitset<8> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return true;
}

// generate operator% for std::bitset<8>
std::bitset<8> modulo(const std::bitset<8> &a, const std::bitset<8> &b)
{
    std::bitset<8> result = a;
    while (result >= b)
    {
        result = subBin(result, b);
    }
    return result;
}

// generate operator* for std::bitset<8>
std::bitset<8> multiply(const std::bitset<8> &a, const std::bitset<8> &b)
{
    std::bitset<8> result(0);
    for (int i = 0; i < 8; ++i)
    {
        if (b[i])
        {
            result = result + (a << i);
        }
    }
    return result;
}

// std::bitset<8> powm(std::bitset<8> base, std::bitset<8> exp, const std::bitset<8> &mod)
// {
//     std::bitset<8> res(1);
//     while (exp > 0)
//     {
//         if (exp[0])
//         {
//             res = (res * base) % mod;
//         }
//         base = (base * base) % mod;
//         exp = exp >> 1; // Create a new bitset for the result of the shift
//     }
//     return res;
// }

// bool baillie_psw(const std::bitset<8> &n)
// {
//     if (n == 2 || n == 3)
//         return true;
//     if (n <= 1 || n[0])
//         return false;

//     std::bitset<8> d = subBin(n, 1);
//     while (!d[0])
//     {
//         d >>= 1;
//     }

//     std::default_random_engine generator;
//     std::uniform_int_distribution<uint64_t> distribution(1, n.to_ullong());

//     for (int i = 0; i < 5; ++i)
//     {
//         std::bitset<8> x = distribution(generator);
//         std::bitset<8> y = powm(x, d, n);
//         if (y != 1 && y != subBin(n, 1))
//         {
//             bool composite = true;
//             for (int r = 1; r < d.count(); ++r)
//             {
//                 y = powm(y, 2, n);
//                 if (y == subBin(n, 1))
//                 {
//                     composite = false;
//                     break;
//                 }
//             }
//             if (composite)
//                 return false;
//         }
//     }

//     return true;
// }

int main()
{
    // test all operator
    std::bitset<8> a(138);
    std::bitset<8> b(31);
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a+b = " << a + b << std::endl;
    std::cout << "a%b = " << modulo(a, b) << std::endl;
    std::cout << "a*b = " << multiply(a, b) << std::endl;
}
