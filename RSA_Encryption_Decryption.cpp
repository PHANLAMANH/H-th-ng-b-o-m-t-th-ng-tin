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
            return false;
        }
    }
    return true;
}



vector <bitset<128>> bezoutCofficient(bitset<128> a, bitset<128> b){
    bitset<128>s (0); bitset<128> old_s = (1);
    bitset<128> t (1); bitset<128>old_t (0);
    bitset<128> r = b; bitset<128> old_r = a;
    
    while ( r != 0){
        bitset<128> quotient = old_r.to_ulong()/r.to_ulong();
        bitset<128> temp = r;
        
        r = old_r.to_ulong() - quotient.to_ulong()*r.to_ulong();
        old_r = temp;
        
        // We treat s and t in the same manner we treated r
        temp = s;
        s = old_s.to_ulong() - quotient.to_ulong()*s.to_ulong();
        old_s = temp;
        
        temp = t;
        t = old_t.to_ulong() - quotient.to_ulong()*t.to_ulong();
        old_t = temp;
    }
    vector<bitset<128>> result;
    result.push_back(old_r); // GCD ( a,b)
    result.push_back(old_s); // x           ax + by = GCD (a,b)
    result.push_back(old_t); // y           ax + by = GCD (a,b)
    
    return result;
}


bitset<128> largePrimeGen( int numbits) {
    srand(static_cast<unsigned int>(time(0)));
    bitset<128> lowerBound = 1ULL << (numbits - 2);
    bitset<128> upperBound = (1ULL << (numbits - 1)) - 1;
    bitset<128> guess = lowerBound.to_ullong() + rand() % (upperBound.to_ullong() - lowerBound.to_ullong() + 1);

    if (guess[0] == 0) {
        guess[0] = 1;
    }

    while (!(isFermatPrime(guess,30))) {
        guess = guess.to_ulong() + 2;
    }
    return guess;
}

vector<bitset<128>> keyGen (bitset<128> p , bitset<128> q){
    
    bitset<128> n = mulBin(p, q);
    cout<<n<<endl;
    bitset<128> phi = subBin(n, subBin(p, subBin(q, one_bitset)));
    
    bitset<128> eCandidate;
    
    while ( true){
        eCandidate = bitset<128>(rand() % (phi.to_ulong() - 2) + 2);// Random integer between 3 and totient - 1
        vector<bitset<128>> bezout = bezoutCofficient(eCandidate, phi);
        if ( bezout[0] == one_bitset){
            vector<bitset<128>> key ;
            key.push_back(bezout[1]);
            key.push_back(bezout[2]);
            key.push_back(n);
            return key;
        }
    }
}
//bitset<128> encrypt ( bitset<128> e,//encrypt key ( public key)
//                     bitset<128> n, //
//                     bitset<128> message){
//    
//}
//
//bitset<128> decrypt ( bitset<128> d,//decrypt key ( private key)
//                     bitset<128> n,
//                     bitset<128> message){
//    
//}

int main(){

    bitset<128> x(85); // 85
    bitset<128> y(53);  // 53
    bitset<128> n("11111");   // 31
    bitset<128> a ("10101010001111101010101010100111010101001010101000111010101110101010101");//1570231049044545592661
    bitset<128> c ("101010100011111010101001010101000111010101110101010101");//11979910757047637
    
    bitset<128> b ("101010100011111010101011011010110010101000111010101110101010101"); //6133715455881076053
    
    bitset<128> test ("0000000000000000000100000000000000000000000000000001111101000110111101011011101101");
//
    bitset<128> p = mulMod(x, y, n);
    cout<<p<<endl;

    bitset<128> q = mulBin(x, y);
    cout<<q<<endl;
//    cout<<gcd;

    
//    bitset<128> p = mulBin(x, n);
//    cout<<p<<endl;
    
//    bitset<128> q = largePrimeGen(128);
//    cout<<q<<endl;
    
    vector<bitset<128>> key = keyGen(x, y);
    cout<<"E: "<<key[0]<<endl;
    
    cout<<"D:"<<key[1]<<endl;
    
    cout<<"N: "<<key[2]<<endl;
    return 0;
}
