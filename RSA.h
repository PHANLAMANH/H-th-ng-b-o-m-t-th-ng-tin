#include "Header.h"

const bitset<128> one_bitset(1);
bitset<128> zero(0);

bool Adder(bool a, bool b, bool &carry)
{
    bool sum = (a ^ b) ^ carry;

    carry = (a && b) || (a && carry) || (b && carry);

    return sum;
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

bitset<128> addBin(bitset<128> &a, bitset<128> &b)
{
    bool carry = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;

    for (int i = 0; i < 128; i++)
    {
        ans[i] = Adder(a[i], b[i], carry);
    }
    return ans;
}

bitset<128> subBin(bitset<128> x, bitset<128> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 128; i++)
    {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

bitset<128> absolutesubBin(bitset<128> x, bitset<128> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 128; i++)
    {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    if (borrow)
    {
        // Compute two's complement (invert all bits and add 1)
        bitset<128> one_bitset(1);
        ans.flip();
        ans = addBin(ans, one_bitset);
    }
    return ans;
}

bitset<128> operator+(const bitset<128> &a, const bitset<128> &b)
{
    bitset<128> result(0);
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

bitset<128> mulBin(bitset<128> &a, bitset<128> &b)
{
    bitset<128> result(0);
    for (int i = 0; i < 128; ++i)
    {
        if (b[i])
        {
            result = result + (a << i);
        }
    }
    return result;
}

bool operator<(bitset<128> &a, bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return b[i];
    }
    return false;
}

bool operator>(bitset<128> &a, bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return false;
}

bool operator>=(bitset<128> &a, bitset<128> &b)
{
    for (int i = 127; i >= 0; i--)
    {
        if (a[i] ^ b[i])
            return a[i];
    }
    return true;
}

bitset<128> Mod(bitset<128> &a, bitset<128> &b)
{
    bitset<128> result = a;

    while (result >= b)
    {
        result = subBin(result, b);
    }
    return result;
}

bitset<128> addMod(bitset<128> x, bitset<128> b, bitset<128> n)
{
    bitset<128> con = addBin(x, b);
    if (con < n)
    {
        return con;
    }
    else
    {
        bitset<128> res = Mod(con, n);
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
        x = Mod(x, n);
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

bool isOdd(bitset<128> x)
{
    if ((x & one_bitset) == 1)
        return true;
    else
        return false;
}

bool isEven(std::bitset<128> x)
{
    bitset<128> one_bitset(1);
    if ((x & one_bitset) == 0)
        return true;
    else
        return false;
}

bitset<128> chooseA(bitset<128> p)
{
    random_device();
    mt19937 gen(rand());
    uniform_int_distribution<> dis(2, p.to_ulong() - 1);
    return dis(gen);
}

bitset<128> extendedEuclid(bitset<128> x, bitset<128> y)
{
    bitset<128> g(1);
    while (isEven(x) && isEven(y))
    {
        x = x >> 1;
        //        cout<<"x: "<<x<<endl;
        y = y >> 1;
        //        cout<<"y: "<<y<<endl;
        g = g << 1;
        //        cout<<"g: "<<g<<endl;
    }
    while (x > zero)
    {
        while (isEven(x))
        {
            x = x >> 1;
        }
        while (isEven(y))
        {
            y = y >> 1;
        }
        bitset<128> t = absolutesubBin(x, y) >> 1;
        if (x >= y)
        {
            x = t;
        }
        else
            y = t;
    }
    g = mulBin(g, y);
    return g;
}

bool isFermatPrime(bitset<128> p, int k)
{
    for (int i = 0; i < k; i++)
    {
        bitset<128> a = chooseA(p);
        if (powerMod(a, subBin(p, one_bitset), p) != 1)
        {
            return false;
        }
    }
    return true;
}

// SAO EM CODE THEO THAY THUC MA KHONG DUOC :<<<<<
vector<bitset<128>> extendedBezout(bitset<128> x, bitset<128> y)
{
    bitset<128> g = 1;
    while (isEven(x) && isEven(y))
    {
        x = x >> 1;
        //        cout<<"x: "<<x<<endl;
        y = y >> 1;
        //        cout<<"y: "<<y<<endl;
        g = g << 1;
        //        cout<<"g: "<<g<<endl;
    }
    bitset<128> u = x;
    bitset<128> v = y;

    bitset<128> A(1);
    bitset<128> B(0);
    bitset<128> C(0);
    bitset<128> D(1);

    while (u > zero)
    {
        while (isEven(u))
        {
            u = u >> 1;
            if (isEven(A) && isEven(B))
            {
                A = A >> 1;
                B = B >> 1;
            }
            else
            {
                A = (addBin(A, y)) >> 1;
                B = (subBin(B, x)) >> 1;
            }
        }
        while (isEven(v))
        {
            v = v >> 1;
            if (isEven(C) && isEven(D))
            {
                C = C >> 1;
                D = D >> 1;
            }
            else
            {
                C = (addBin(C, y)) >> 1;
                D = (subBin(D, x)) >> 1;
            }
        }
        if (u >= v)
        {
            u = subBin(u, v);
            A = subBin(A, C);
            B = subBin(B, D);
        }
        else
        {
            v = subBin(v, u);
            C = subBin(C, A);
            D = subBin(D, B);
        }
    }
    bitset<128> a = C;
    bitset<128> b = D;
    bitset<128> gcd = mulBin(g, v);

    vector<bitset<128>> key;
    key.push_back(gcd);
    key.push_back(a);
    key.push_back(b);
    return key;
}

pair<bitset<128>, bitset<128>> divideBitsets(const bitset<128> &dividend, bitset<128> &divisor)
{
    // Ensure divisor is not zero
    if (divisor == 0)
    {
        cerr << "Error: Division by zero!" << endl;
        return make_pair(bitset<128>(0), bitset<128>(0));
    }

    bitset<128> quotient(0);
    bitset<128> remainder(0);

    for (int i = 127; i >= 0; --i)
    {
        remainder <<= 1;            // Left shift to make room for the next bit
        remainder[0] = dividend[i]; // Add the next bit from the dividend

        if (remainder >= divisor)
        {
            remainder = subBin(remainder, divisor);
            quotient[i] = 1;
        }
    }

    return make_pair(quotient, remainder);
}

vector<bitset<128>> bezoutCofficient(bitset<128> a, bitset<128> b)
{
    bitset<128> s(0);
    bitset<128> old_s = (1);
    bitset<128> t(1);
    bitset<128> old_t(0);
    bitset<128> r = b;
    bitset<128> old_r = a;

    while (r != 0)
    {
        bitset<128> quotient = divideBitsets(old_r, r).first;
        //        old_r.to_ullong() / r.to_ullong();
        bitset<128> temp = r;

        r = subBin(old_r, mulBin(quotient, r));
        // old_r.to_ulong() - quotient.to_ulong() * r.to_ulong();
        old_r = temp;

        // We treat s and t in the same manner we treated r
        temp = s;
        s = subBin(old_s, mulBin(quotient, s));
        // old_s.to_ulong() - quotient.to_ulong()*s.to_ulong();
        old_s = temp;

        temp = t;
        t = subBin(old_t, mulBin(quotient, t));
        old_t = temp;
    }
    vector<bitset<128>> result;
    result.push_back(old_r); // GCD ( a,b)
    result.push_back(old_s); // x           ax + by = GCD (a,b)
    result.push_back(old_t); // y           ax + by = GCD (a,b)

    return result;
}

bitset<128> generateRandomBitset()
{
    random_device rd;
    mt19937 gen(rand());
    uniform_int_distribution<> dis(numeric_limits<int>::min(), numeric_limits<int>::max());

    uint64_t randomNumber = dis(gen);
    return bitset<128>(randomNumber);
}

bitset<128> generateLargePrime()
{
    bitset<128> n;
    do
    {
        n = generateRandomBitset();
    } while (!isFermatPrime(n, 10));

    return n;
}

vector<bitset<128>> keyGen(bitset<128> p, bitset<128> q)
{

    bitset<128> n = mulBin(p, q);
    //    cout << n << endl;
    bitset<128> p_1 = subBin(p, one_bitset);
    bitset<128> q_1 = subBin(q, one_bitset);
    bitset<128> phi = mulBin(p_1, q_1);
    cout << phi << endl;

    bitset<128> eCandidate, d;
    do
    {
        // Random value for e, 1 < e < totient
        bitset<128> random = bitset<128>(rand());
        bitset<128> temp = phi + bitset<128>(2);
        eCandidate = Mod(random, temp);
    } while (extendedEuclid(eCandidate, phi) != 1);

    d = bezoutCofficient(eCandidate, phi)[1];

    // TO MAKE SURE NOT NEGATIVE
    phi = phi << 1;
    d = addBin(d, phi);

    vector<bitset<128>> key;
    key.push_back(eCandidate);
    key.push_back(d);
    key.push_back(n);
    return key;
}

bitset<128> encrypt_RSA(bitset<128> e, // encrypt key ( public key)
                        bitset<128> n, //
                        bitset<128> k)
{
    return powerMod(k, e, n);
}

bitset<128> decrypt_RSA(bitset<128> d, // decrypt key ( private key)
                        bitset<128> n,
                        bitset<128> c)
{
    return powerMod(c, d, n);
}
void writeEncryptKeyToFile(const std::string &filename, const std::vector<std::bitset<128>> &key)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }
    outputFile << key[0] << endl;
    outputFile << key[2];

    outputFile.close();
    cout << "Key has been successfully written to the file: " << filename << std::endl;
}

void writeDecryptKeyToFile(const std::string &filename, const std::vector<std::bitset<128>> &key)
{
    std::ofstream outputFile(filename);

    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }
    outputFile << key[1] << endl;
    outputFile << key[2];

    outputFile.close();
    cout << "Key has been successfully written to the file: " << filename << std::endl;
}