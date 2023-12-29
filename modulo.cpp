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

bool operator<(const std::bitset<128> &a, const std::bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return b[i];
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
bitset<128> mulBin(bitset<128> &a, bitset<128> &b)
{
    bitset<128> result(0);

    for (int i = 0; i < 127; ++i)
    {
        if (b[i] == 1)
        {
            result ^= (a << i);
        }
    }
    return result;
}

bitset<128> addMod(bitset<128> x, bitset<128> b, bitset<128> n)
{
    bitset<128> con = x + b;
    if (con < n)
    {
        return con;
    }
    else
    {
        bitset<128> res = modulo(con, n);
        return res;
    }
}

bitset<128> mulMod(bitset<128> x, bitset<128> y, bitset<128> n)
{
    bitset<128> P(0);

    if (y[0] == 1)
    {
        P = x;
    }

    for (int i = 1; i < 127; i++)
    {
        // x ≡ 2*x (mod n)
        x <<= 1;
        x = modulo(x, n);

        // z = yi * x

        bitset<128> yi(y[i]);
        bitset<128> z = mulBin(yi, x);

        // P ≡ (P + z) (mod n)
        P = addMod(P, z, n);
    }
    return P;
}

bitset<128> powerMod(bitset<128> x, bitset<128> a, bitset<128> n)
{
    bitset<128> y(1);

    for (int i = 127; i >= 0; i--)
    {
        y = mulMod(y, y, n);
        if (a[i] == 1)
        {
            y = mulMod(y, x, n);
        }
    }
    return y;
}

bool miller_rabin(const std::bitset<128> &n)
{
    std::bitset<128> a;
    std::bitset<128> n_1 = subBin(n, 1);
    std::bitset<128> d = n_1;
    int s = 0;
    while (d[0] == 0)
    {
        d >>= 1;
        s++;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, n_1.to_ulong());
    for (int i = 0; i < 10; i++)
    {
        a = dis(gen);
        std::bitset<128> x = powerMod(a, d, n);
        if (x == 1 || x == n_1)
        {
            continue;
        }
        for (int j = 0; j < s - 1; j++)
        {
            x = mulMod(x, x, n);
            if (x == 1)
            {
                return false;
            }
            if (x == n_1)
            {
                break;
            }
        }
        if (x != n_1)
        {
            return false;
        }
    }
    return true;
}

int main()
{

    std::bitset<128> n("10010001001110011011010001110101");
    if (miller_rabin(n))
    {
        std::cout << n << " is a prime number" << std::endl;
    }
    else
    {
        std::cout << n << " is not a prime number" << std::endl;
    }
    return 0;
}
