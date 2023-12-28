#include <iostream>
#include <bitset>
#include <random>
using namespace std;

// generate operator<= for std::bitset<128>
bool operator<=(const std::bitset<128> &a, const std::bitset<128> &b)
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

bitset<128> subBin(bitset<128> x, bitset<128> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 7; i++)
    {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

// generate operator> for std::bitset<128>
bool operator>(const std::bitset<128> &a, const std::bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return false;
}

// generate operator+ for std::bitset<128>
std::bitset<128> operator+(const std::bitset<128> &a, const std::bitset<128> &b)
{
    std::bitset<128> result(0);
    bool carry = false;
    for (int i = 0; i < 128; ++i)
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

//  std::bitset<128> operator=(const std::bitset<128> &a)
//  {
//      std::bitset<128> result(0);
//      for (int i = 0; i < 128; ++i)
//      {
//          result[i] = a[i];
//      }
//      return result;
//  }

// generate operator>= for std::bitset<128>
bool operator>=(const std::bitset<128> &a, const std::bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return true;
}

// generate operator>>= for std::bitset<128>
std::bitset<128> operator>>=(std::bitset<128> &a, int n)
{
    std::bitset<128> result(0);
    for (int i = 0; i < 128; ++i)
    {
        if (i + n < 128)
        {
            result[i] = a[i + n];
        }
    }
    return result;
}

// generate operator% for std::bitset<128>
std::bitset<128> modulo(const std::bitset<128> &a, const std::bitset<128> &b)
{
    std::bitset<128> result = a;
    while (result >= b)
    {
        result = subBin(result, b);
    }
    return result;
}

// generate operator* for std::bitset<128>
std::bitset<128> multiply(const std::bitset<128> &a, const std::bitset<128> &b)
{
    std::bitset<128> result(0);
    for (int i = 0; i < 128; ++i)
    {
        if (b[i])
        {
            result = result + (a << i);
        }
    }
    return result;
}

std::bitset<128> powerModulo(const std::bitset<128> &base, std::bitset<128> exp, const std::bitset<128> &mod)
{
    std::bitset<128> result(1);
    std::bitset<128> tempBase = base;
    while (exp > 0)
    {
        if (exp[0])
        {
            result = modulo(multiply(result, tempBase), mod);
        }
        tempBase = modulo(multiply(tempBase, tempBase), mod);
        exp = exp >> 1;
    }
    return result;
}
bool baillie_psw(const std::bitset<128> &n)
{
    if (n == 2 || n == 3)
        return true;
    if (n <= 1 || n[0])
        return false;

    std::bitset<128> d = subBin(n, 1);
    while (!d[0])
    {
        d >>= 1;
    }

    std::default_random_engine generator;
    std::uniform_int_distribution<uint64_t> distribution(1, n.to_ullong());

    for (int i = 0; i < 5; ++i)
    {
        std::bitset<128> x = distribution(generator);
        std::bitset<128> y = powerModulo(x, d, n);
        if (y != 1 && y != subBin(n, 1))
        {
            bool composite = true;
            for (int r = 1; r < d.count(); ++r)
            {
                y = powerModulo(y, 2, n);
                if (y == subBin(n, 1))
                {
                    composite = false;
                    break;
                }
            }
            if (composite)
                return false;
        }
    }

    return true;
}

int main()
{
    // test powm function
    std::bitset<128> base(2);
    std::bitset<128> exp(3);
    std::bitset<128> mod(5);
    std::bitset<128> result = powerModulo(base, exp, mod);
    std::cout << result << std::endl;
    // check prime number
    std::bitset<128> n(97);
    std::cout << baillie_psw(97) << std::endl;
    if (baillie_psw(n))
    {
        std::cout << "prime" << std::endl;
    }
    else
    {
        std::cout << "composite" << std::endl;
    }
}
