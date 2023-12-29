#include "Header.h"

const bitset<128> one_bitset(1);
const bitset<128> two_bitset(2);
const bitset<128> zero(0);

bool Adder(bool a, bool b, bool &carry){
    bool sum = (a ^ b) ^ carry;

    carry = (a && b) || (a && carry) || (b && carry);

    return sum;
}

bool Subtracter(bool a, bool b, bool &borrow){
    bool diff;

    if (borrow){
        diff = !(a ^ b);
        borrow = !a || (a && b);
    }

    else{
        diff = a ^ b;
        borrow = !a && b;
    }
    return diff;
}

bitset<128> addBin(bitset<128> &a, bitset<128> &b){
    bool carry = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;

    for (int i = 0; i < 127; i++){
        ans[i] = Adder(a[i], b[i], carry);
    }
    return ans;
}

bitset<128> subBin(bitset<128> x, bitset<128> y){
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 127; i++){
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

bitset<128> absolutesubBin(bitset<128> x, bitset<128> y){
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 127; i++){
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    if (borrow) {
        // Compute two's complement (invert all bits and add 1)
        bitset<128> one_bitset(1);
        ans.flip();
        ans = addBin(ans, one_bitset);
    }
    return ans;
}

bitset<128> mulBin(bitset<128> &a, bitset<128> &b){
    bitset<128> result(0);

    for (int i = 0; i < 127; ++i){
        if (b[i] == 1){
            result ^= (a << i);
        }
    }
    return result;
}

bool operator<(bitset<128> &a, bitset<128> &b){
    for (int i = 127; i >= 0; i--){
        if (a[i] ^ b[i])
            return b[i];
    }
    return false;
}

bool operator>(bitset<128> &a, bitset<128> &b){
    for (int i = 127; i >= 0; i--){
        if (a[i] ^ b[i])
            return a[i];
    }
    return false;
}

bool operator>=(bitset<128> &a, bitset<128> &b){
    for (int i = 127; i >= 0; i--){
        if (a[i] ^ b[i]) return a[i];
    }
    return true;
}

bitset<128> Mod(bitset<128> &a, bitset<128> &b){
    bitset<128> result = a;
    
    while (result >= b){
        result = subBin(result, b);
    }
    return result;
}

bitset<128> addMod(bitset<128> x, bitset<128> b, bitset<128> n){
    bitset<128> con = addBin(x, b);
    if (con < n){
        return con;
    }
    else{
        bitset<128> res = Mod(con, n);
        return res;
    }
}

bitset<128> mulMod(bitset<128> x, bitset<128> y, bitset<128> n){
    bitset<128> P(0);

    if (y[0] == 1){
        P = x;
    }

    for (int i = 1; i < 127; i++){
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

bitset<128> powerMod(bitset<128> x, bitset<128> a, bitset<128> n){
    bitset<128> y(1);

    for (int i = 127; i >= 0; i--){
        y = mulMod(y, y, n);
        if (a[i] == 1){
            y = mulMod(y, x, n);
        }
    }
    return y;
}

bool isOdd ( bitset<128> x){
    if ((x & one_bitset) == 1) return true;
        else return false;
}

bool isEven ( bitset<128> x){
    if ((x & one_bitset) == 0) return true;
        else return false;
}
        
bitset<128> findGCD( bitset<128> a, bitset<128> b){
    bitset<128> g(1);
    bitset<128> two(2);
    bitset<128> zero(0);
    while ( isEven(a) && isEven(b)){
        a = a >> 1;
        cout<<"a: "<<a<<endl;
        b = b >> 1;
        cout<<"b:" << b<<endl;
        g = mulBin(g, two);
    }
    while ( a > zero){
        cout<<a<<endl;
        while (isEven(a)){
            bool even = isEven(a);
            cout<<even<<endl;
            cout<<"a: "<<a<<endl;
            a = a >> 1;
        }
        while (isEven(b)){
            cout<<"b: "<<b<<endl;
            b = b >> 1;
        }
        bitset<128> t = absolutesubBin(a, b);
        cout<<"T: "<<t<<endl;
        if (a > b ) a = t;
        else b = t;
    }
    g = mulBin(g, b);
    return g;
}

bitset<128 > chooseA( bitset<128> p){
    random_device();
    mt19937 gen(rand());
    uniform_int_distribution<> dis(ul(2), subBin(p, one_bitset).to_ulong());
    return dis(gen);
}

bool isFermatPrime( bitset<128> p, int k){
    for ( int i = 0 ; i < k; i++){
        bitset<128> a = chooseA(p);
        if ( powerMod(a, subBin(p, one_bitset), p) != 1){
            cout<<"Composite"<<endl;
            return false;
        }
    }
    cout<<"Pseudoprime"<<endl;
    return true;
}



int main()
{

    bitset<128> x("1010101"); // 85
    bitset<128> y("110101");  // 53
    bitset<128> n("11111");   // 31
    bitset<128> p(41);

//    bitset<128> gcd = findGCD(x, y);
//    cout<<y<<endl;
//    cout<<gcd;
    
    bool prime = isFermatPrime(n, 30);
    cout<<prime;
    
    return 0;
}
