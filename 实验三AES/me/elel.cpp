#include <bits/stdc++.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef uint8_t word8;
typedef pair<uint8_t, uint8_t> pii;

// circular shift
#define ROLT8( x, shift ) ( ( word8 ) ( ( x ) << ( shift ) ) | ( ( x ) >> ( 8 - ( shift ) ) ) )
#define GROUP_SIZE 255

word8 Log[ 1 << 8 ]; // a = g^i,then Log[ a ] = i;
word8 Exp[ 1 << 8 ]; // a = g^i, then Exp[ i ] = a;
word8 Sbox[ 1 << 8 ];

/*
 * 加法
 */
word8 add_or_sub ( word8 a, word8 b ) { return a ^ b; }

/*
 * 基本的移位乘法
 */
word8 mul0 ( word8 a, word8 b ) {
    word8 res = 0;
    while ( b ) {
        if ( b & 1 )
            res = res ^ a;
        a = ( a << 1 ) ^ ( a & 0x80 ? 0x11b : 0 );
        b >>= 1;
    }
    return res;
}

word8 div0 ( word8 a ) {
    if ( a & 1 )
        a = ( a ^ 0x11b ) >> 1;
    else
        a >>= 1;
    return a;
}

char GetHigh ( word8 num ) {
    for ( char i = 15; i >= 0; --i ) {
        if ( num >> i )
            return i;
    }
    return -1;
}

word8 divide_lal ( word8 x, word8 y, word8 &tmp ) {

    word8 result = 0;
    word8 y_High = GetHigh ( y );
    char diff = GetHigh ( x ) - y_High;
    if ( diff < 0 ) {
        tmp = x;
        return 0;
    }

    while ( diff >= 0 ) {
        result |= ( word8 ) ( 1 << diff );
        x = ( x ^ ( word8 ) ( y << diff ) );
        diff = GetHigh ( x ) - y_High;
    }

    tmp = x;
    return result;
}

word8 egcd ( word8 a, word8 b, word8 &x, word8 &y ) {
    if ( b == 0 ) {
        x = 1, y = 0;
        return a;
    }
    word8 r;
    word8 q = divide_lal ( a, b, r );
    word8 g = egcd ( b, r, y, x );
    y -= x * q;
    return g;
}

int main () {
    word8 r = 0;

    return 0;
}
