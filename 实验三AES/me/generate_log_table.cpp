// https://crypto.stackexchange.com/questions/21173/how-to-calculate-aes-logarithm-table
// http://www.cs.utsa.edu/~wagner/laws/FFM.html

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
    return a;
}

/*
 * 建立指数和对数两个表,生成元取3
 */
void generate_log_table () {
    word8 g = 3;
    word8 a = 1;
    // g^i = a;
    for ( int i = 0; i <= GROUP_SIZE; ++i ) {
        Exp[ i ] = a;
        Log[ a ] = i % GROUP_SIZE; // g^0 = g^GROUP_SIZE = 1,取0
        a = mul0 ( a, g );
    }
}

char GetHigh ( word8 num ) {
    for ( char i = 15; i >= 0; --i ) {
        if ( num >> i )
            return i;
    }
    return -1;
}

word8 divide_lal ( word8 x, word8 y, int &tmp ) {

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

word8 egcd ( int a, int b, int &x, int &y ) {
    if ( b == 0 ) {
        x = 1, y = 0;
        return a;
    }
    int r;
    int q = divide_lal ( a, b, r );

    int g = egcd ( b, r, y, x );

    y ^= mul0 ( x, q );
    // printf ( "egcd: a:%d b:%d x:%d y:%d q:%d\n", a, b, x, y, q );
    return g;
}

/*
 * 求离散对数
 */
word8 discrete_log ( word8 a ) {
    generate_log_table ();
    return Log[ a ];
}

/*
 * 先打表后真正使用的乘法
 */
word8 mul_neo ( word8 a, word8 b ) {
    if ( a && b )
        return Exp[ Log[ a ] + Log[ b ] ];
    else
        return 0;
}

//逆元
word8 inverse_mul ( word8 a ) {
    if ( a )
        return Exp[ GROUP_SIZE - Log[ a ] ];
    else
        return 0;
}

/*
 * 不需要先打对数的表
 * 一边计算Sbox
 * 同时打对数的表
 */
void init_sbox_and_logtable () {
    word8 p = 1, q = 1, i = 0;
    do {
        Log[ i ] = p;
        Exp[ p ] = i++;

        // p = p * 3,生成整个群
        p = p ^ ( p << 1 ) ^ ( p & 0x80 ? 0x1b : 0 );

        // q is multiplicative inverse of p , q = q/3;
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;

        word8 xformed = q ^ ROLT8 ( q, 1 ) ^ ROLT8 ( q, 2 ) ^ ROLT8 ( q, 3 ) ^ ROLT8 ( q, 4 );
        Sbox[ p ] = xformed ^ 0x63;
    } while ( p != 1 );
    Sbox[ 0 ] = 0x63;
}

/*
 * 先打对数的表用作乘法逆元
 * 再生成S_box
 */
void init_twice () {
    word8 p, q;
    generate_log_table ();
    for ( int i = 1; i <= GROUP_SIZE; ++i ) {
        p = i;
        q = Exp[ GROUP_SIZE - Log[ p ] ];

        word8 xformed = q ^ ROLT8 ( q, 1 ) ^ ROLT8 ( q, 2 ) ^ ROLT8 ( q, 3 ) ^ ROLT8 ( q, 4 );
        Sbox[ p ] = xformed ^ 0x63;
    }
    Sbox[ 0 ] = 0x63;
}

/*
 * 比较过后
 * 两个计算方法时间近似
 */

int main () {
    generate_log_table ();
    int i = 2;
    int x, y;
    egcd ( 283, i, x, y );
    printf ( "%d %d\n", inverse_mul ( i ), y );

    init_sbox_and_logtable ();
    for ( int i = 0; i < 256; ++i )
        printf ( "%x ", Sbox[ i ] );

    return 0;
}
