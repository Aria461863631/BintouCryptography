# 实验课程作业3-AES相关

编程题：

1. 用C／C++实现对GF(2^8)的若干运算功能，包括：加法、乘法、求乘法逆元（给定x，求y，使得x*y == 1 )、求离散对数（即给定一个生成元g，输入x，求y使得g^y == x) 。
2. 编程实现AES的S-box的生成。

WARNING:

- 师兄说运算不能查表的，除法要egcd来的，没更新，具体看ECC那里吧

```c
// https://crypto.stackexchange.com/questions/21173/how-to-calculate-aes-logarithm-table
// http://www.cs.utsa.edu/~wagner/laws/FFM.html

#include <bits/stdc++.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t word8;

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
 * 约0.000015s
 */

```