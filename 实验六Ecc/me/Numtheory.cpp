#include <algorithm>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

#ifdef LOCAL
#define dbg( args... ) \
    do {               \
        err ( args );  \
    } while ( 0 )
#else
#define dbg( ... )
#endif
#ifdef ONLINE_JUDGE
#define assert( condition )    \
    if ( !( condition ) ) {    \
        int x = 1, y = 0;      \
        cout << x / y << endl; \
    }
#endif

void err () { cout << "" << endl; }
template <template <typename...> class T, typename t, typename... Args>
void err ( T<t> a, Args... args ) {
    for ( auto x : a )
        cout << x << ' ';
    err ( args... );
}
template <typename T, typename... Args>
void err ( T a, Args... args ) {
    cout << a << ' ';
    err ( args... );
}
//-----------------------------------------------------------

typedef long long ll;

const int INF = 0x7f7f7f7f;
const int maxn = 1000 + 10;

int gcd ( int a, int b ) { return ( b == 0 ) ? a : gcd ( b, a % b ); }

int lcm ( int a, int b ) { return a * b / gcd ( a, b ); }

int prime[ maxn ];

void getPrime () {
    memset ( prime, 0, sizeof ( prime ) );
    for ( int i = 2; i < maxn; ++i ) {
        if ( !prime[ i ] )
            prime[ ++prime[ 0 ] ] = i;
        for ( int j = 1; j <= prime[ 0 ] && prime[ j ] <= maxn / i; ++j ) {
            prime[ prime[ j ] * i ] = 1;
            if ( i % prime[ j ] == 0 )
                break;
        }
    }
}

int facCnt;
int getFactors ( ll x, int factor[][ 2 ] ) {
    if ( prime[ 0 ] == 0 )
        getPrime ();

    facCnt = 0;
    ll tmp = x;
    for ( int i = 1; prime[ i ] <= tmp / prime[ i ]; ++i ) {
        factor[ facCnt ][ 1 ] = 0;
        if ( tmp % prime[ i ] == 0 ) {
            factor[ facCnt ][ 0 ] = prime[ i ];
            while ( tmp % prime[ i ] == 0 ) {
                factor[ facCnt ][ 1 ]++;
                tmp /= prime[ i ];
            }
            facCnt++;
        }
    }
    if ( tmp != 1 ) {
        factor[ facCnt ][ 0 ] = tmp;
        factor[ facCnt++ ][ 1 ] = 1;
    }
    return facCnt;
}

int getDivisors ( int num, int divisor[] ) {
    int c = 0;
    int square_root = (int)sqrt ( num ) + 1;
    for ( int i = 1; i < square_root; i++ ) {
        if ( num % i == 0 && i * i != num )
            divisor[ c++ ] = i, divisor[ c++ ] = num / i;
        if ( num % i == 0 && i * i == num )
            divisor[ c++ ] = i;
    }
    sort ( divisor, divisor + c );
    return c;
}

int prime_to_m_part ( int n, int m ) {
    int g = gcd ( n, m );
    while ( n % g == 0 )
        n /= g;
    return n;
}
