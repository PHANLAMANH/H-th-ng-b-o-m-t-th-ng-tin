//
//  RSA_Encryption_Decryption.cpp
//  Information_Security
//
//  Created by minnie on 27/12/2023.
//

#include "Header.h"


bool Adder ( bool a, bool b, bool &carry){
    bool sum  = (a ^ b) ^ carry;
    carry = (a && b) || (a && carry) || (b && carry);
    return sum;
}

bool Subtracter( bool a , bool b, bool & borrow){
    bool diff;
        if (borrow) {
            diff = !(a ^ b);
            borrow = !a || (a && b);
        }
        else {
            diff = a ^ b;
            borrow = !a && b;
        }
        return diff;
}

bitset<128> addBin(bitset<128> &a, bitset<128> &b){
    bool carry = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 127; i++) {
        ans[i] = Adder(a[i], b[i], carry);
    }
    return ans;
}

bitset<128> subBin(bitset<128> x, bitset<128> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<128> ans;
    for (int i = 0; i < 127; i++) {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

bitset<128> mulBin(bitset<128>& a,bitset<128>& b) {
    bitset<128> result(0);

    for (int i = 0; i < 127; ++i) {
        if (b[i] == 1) {
            result ^= (a << i);
        }
    }
    return result;
}

bool operator< ( bitset<128> &a, bitset <128> &b){
    for ( int i = 127; i >= 0; i--){
        if ( a[i] ^ b[i]) return b[i];
    }
    return false;
}

bool operator> ( bitset<128> &a, bitset <128> &b){
    for ( int i = 127; i >= 0; i--){
        if ( a[i] ^ b[i]) return a[i];
    }
    return false;
}

bitset<128> addMol(bitset<128> x, bitset<128> b , bitset<128> n){
    bitset<128> con = addBin(x, b);
    if ( con < n){
        return con;
    }
    else {
        bitset<128> res = subBin(con, n);
        return res;
    }
}

bitset<128> mulMol(bitset<128> x, bitset<128> y , bitset<128> n){
    bitset<128> P (0);
    if ( x[0] == 1){
        for ( int i=0; i < 255;i++){
            P[i] = x[i];
        }
        return P;
    }
    
    for ( int i=1 ; i < 255; i++){
        //x ≡ 2*x (mod n)
        x <<= 1;
        bitset<128> one_bitset(1);
        x &= subBin(n, one_bitset); // modulo n
        
        // z = yi * x
        bitset<128> yi (y[i]);
        bitset<128> z = mulBin(yi,x);
        
        P = addMol(P,z,n);
    }
    
    return P;
}

int main(){
//    bitset<128> a(12);
//    cout<<a<<endl;
//    bitset<128> b(15);
//    
//    bitset<129> result = addBin(a, b);
//    cout<<result;
//    bitset<128> one_bitset(10);
//    bitset<128> d(2);
//    bitset<128> e = mulBin(d,one_bitset);
//    cout<<e;
    bitset<128> x("1010101"); // 85
    bitset<128> y("110101"); //53
    bitset<128> n("11111"); //31
    
    
    bitset<128> d = addMol(x,y,n);
    cout<<d<<endl;
    
    
    
//    cout<<x<<"\t"<<y<<"\t"<<n<<endl;
    
//    
//    bitset<128> result = mulMol(x, y, n);
//    cout << "Result: " << result << std::endl;
    return 0;
}
