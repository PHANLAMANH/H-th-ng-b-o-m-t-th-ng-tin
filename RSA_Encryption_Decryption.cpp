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


bitset<129> addBit(bitset<128> &a, bitset<128> &b){
    bool carry = false;
    // bitset to store the sum of the two bitsets
    bitset<129> ans;
    for (int i = 0; i < 129; i++) {
        ans[i] = Adder(a[i], b[i], carry);
    }
    return ans;
}

bitset<129> subBit(bitset<129> x, bitset<128> y)
{
    bool borrow = false;
    // bitset to store the sum of the two bitsets
    bitset<129> ans;
    for (int i = 0; i < 129; i++) {
        ans[i] = Subtracter(x[i], y[i], borrow);
    }
    return ans;
}

//bitset<16> binaryMultiplication(bitset<8> num1, bitset<8> num2) {
//    bitset<16> result(0);
//
//    for (int i = 0; i < 8; ++i) {
//        if (num2[i] == 1) {
//            result ^= (num1 << i);  // XOR operation to add the shifted partial product
//        }
//    }
//
//    return result;
//}


bitset<9> compareBin(bitset<9> num1, std::bitset<9> num2) {
    for (int i = 8; i >= 0; --i) {
        if (num1[i] > num2[i]) {
            return num1;
        } else if (num2[i] > num1[i]) {
            return num2;
        }
    }
    return num1;
}
bitset<129> addMol(bitset<128> a, bitset<128> b , bitset<128> n){
    bitset<129> con = addBit(a, b);
    if ( con == 0){
        return con;
    }
    else {
        bitset<129> res = subBit(con, n);
        return res;
    }
}


int main(){
    bitset<128> a(12);
    cout<<a<<endl;
    bitset<128> b(15);
    
    bitset<129> result = addBit(a, b);
    cout<<result;
    return 0;
}
