#include <algorithm>
#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;
typedef long long ll;

const int INF = 2e9;
const int maxn = 1e5 + 10;

// x^y mod MOD
int Pow ( int x, int y, int mod ) {
    int ans = 1;
    for ( ; y; y >>= 1, x = 1LL * x * x % mod )
        if ( y & 1 )
            ans = 1LL * ans * x % mod;
    return ans;
}
int Pow ( int x, int y ) { return Pow ( x, y, INF ); }

// GCD
int gcd ( int a, int b ) { return b ? gcd ( b, a % b ) : a; }

// EXGCD
int exgcd ( int a, int b, int &x, int &y ) {
    if ( !b ) {
        x = 1, y = 0;
        return a;
    }
    int res = exgcd ( b, a % b, y, x );
    y -= ( a / b ) * x;
    return res;
}

//素数打表,pcnt代表n之内素数的个数,Prime存素数有哪些,f为bool值只在当前打表时有用
int pcnt, f[ maxn ], Prime[ maxn ];
void Get_Prime ( int n ) {
    memset ( f, 0, sizeof ( f ) );
    pcnt = 0;
    for ( int i = 2; i <= n; ++i ) {
        if ( f[ i ] )
            continue;
        Prime[ ++pcnt ] = i;
        for ( int j = i + i; j <= n; j += i )
            f[ j ] = 1;
    }
}

//把x素因子分解,p数组存素因子,q数组存对应的幂次,cnt代表素因子的个数
void Prime_Decomposite ( int x, int *p, int *q, int &cnt ) {
    cnt = 0;
    for ( int i = 1; i <= pcnt && Prime[ i ] * Prime[ i ] <= x; ++i ) {
        if ( x % Prime[ i ] )
            continue;
        p[ ++cnt ] = Prime[ i ], q[ cnt ] = 0;
        while ( x % p[ cnt ] == 0 )
            x /= p[ cnt ], q[ cnt ]++;
    }
    if ( x > 1 )
        p[ ++cnt ] = x, q[ cnt ] = 1;
}

//求P^C的原根
int Fac[ 50 ], Fac_cnt = 0;
bool Get_g_Check ( int P, int C, int x ) {
    int phi = Pow ( P, C - 1 ) * ( P - 1 ), pw = Pow ( P, C );
    if ( C > 1 && Pow ( x, phi / P, pw ) == 1 )
        return 0;
    for ( int i = 1; i <= Fac_cnt; i++ )
        if ( Pow ( x, phi / Fac[ i ], pw ) == 1 )
            return 0;
    return 1;
}
int Get_Primitive_Root ( int P, int C ) {
    int v = P - 1;
    Fac_cnt = 0;
    for ( int i = 1; i <= pcnt && Prime[ i ] * Prime[ i ] <= v; i++ )
        if ( v % Prime[ i ] == 0 ) {
            Fac[ ++Fac_cnt ] = Prime[ i ];
            while ( v % Prime[ i ] == 0 )
                v /= Prime[ i ];
        }
    if ( v > 1 )
        Fac[ ++Fac_cnt ] = v;
    for ( int i = 2;; i++ )
        if ( Get_g_Check ( P, C, i ) )
            return i;
    return -1;
}

// BSGS求离散对数:x^A = B ( mod P )
struct hash_map {
    static const int Ti = 233, mod = 1 << 16;
    int cnt, k[ mod + 1 ], v[ mod + 1 ], nxt[ mod + 1 ], fst[ mod + 1 ];
    int Hash ( int x ) {
        int v = x & ( mod - 1 );
        return v == 0 ? mod : v;
    }
    void clear () {
        cnt = 0;
        memset ( fst, 0, sizeof fst );
    }
    void update ( int x, int a ) {
        int y = Hash ( x );
        for ( int p = fst[ y ]; p; p = nxt[ p ] )
            if ( k[ p ] == x ) {
                v[ p ] = a;
                return;
            }
        k[ ++cnt ] = x, nxt[ cnt ] = fst[ y ], fst[ y ] = cnt, v[ cnt ] = a;
        return;
    }
    int find ( int x ) {
        int y = Hash ( x );
        for ( int p = fst[ y ]; p; p = nxt[ p ] )
            if ( k[ p ] == x )
                return v[ p ];
        return 0;
    }
    int &operator[] ( int x ) {
        int y = Hash ( x );
        for ( int p = fst[ y ]; p; p = nxt[ p ] )
            if ( k[ p ] == x )
                return v[ p ];
        k[ ++cnt ] = x, nxt[ cnt ] = fst[ y ], fst[ y ] = cnt;
        return v[ cnt ] = 0;
    }
} Map;
int BSGS ( int A, int B, int P ) {
    int M = max ( (int)( 0.8 * sqrt ( 1.0 * P ) ), 1 ), AM = Pow ( A, M, P );
    Map.clear ();
    for ( int b = 0, pw = B; b < M; b++, pw = 1LL * pw * A % P )
        Map.update ( pw, b + 1 );
    for ( int a = M, pw = AM; a - M < P; a += M, pw = 1LL * pw * AM % P ) {
        int v = Map.find ( pw );
        if ( v )
            return a - ( v - 1 );
    }
    return -1;
}

// X^A = B ( mod P^C ),其中B和P^C互素
// flag = 0:返回解的个数, flag = 1:返回解
int RHD ( vector<int> &v, int A, int B, int P, int C, int flag ) {
    //转换为指标求解:A * ind(X) = ind(B) ( mod phi(P^C) )
    int g = Get_Primitive_Root ( P, C );
    int t = BSGS ( g, B, Pow ( P, C ) ); // t = ind(B)
    int mod = ( P - 1 ) * Pow ( P, C - 1 ), pw = mod / ( P - 1 ) * P;
    int GCD = gcd ( t, gcd ( A, mod ) ); //保证gcd(A,mod)|t,即gcd(A,mod,t)=gcd(A,mod)

    if ( !flag )
        return gcd ( A, mod ) > GCD ? 0 : GCD;

    v.clear ();
    if ( gcd ( A, mod ) > GCD ) // GCD == gcd(A,mod)可解
        return 0;

    // exgcd求解
    int _A = A / GCD, _t = t / GCD, _mod = mod / GCD;
    int x, y;
    exgcd ( _A, _mod, y, x ); //_Ay = 1 ( mod _mod )
    y = ( y % _mod + _mod ) % _mod;
    y = 1ll * y * _t % _mod;              //第一个可行解
    for ( int i = y; i < mod; i += _mod ) //一共GCD个可行解
        v.push_back ( Pow ( g, i, pw ) );
    return GCD;
}

// X^A = B ( mod 2^C )
vector<int> tmp;
void dfs ( vector<int> &v, int A, int B, int C, int t, int d ) {
    if ( Pow ( t, A, 1 << d ) != B % ( 1 << d ) )
        return;
    if ( C == d )
        return v.push_back ( t );
    dfs ( v, A, B, C, t, d + 1 );              // t
    dfs ( v, A, B, C, t | ( 1 << d ), d + 1 ); // t + 2^(d)
}
int solve2 ( vector<int> &v, int A, int B, int C ) {
    v.clear ();
    B %= 1 << C;
    dfs ( v, A, B, C, 0, 0 ); // base case: 0^A = B ( mod 1 )
    return (int)v.size ();
}

// x^A = B ( mod P^C )
// flag = 0:返回解的个数, flag = 1:返回解
int solve ( vector<int> &v, int A, int B, int P, int C, int flag ) {
    if ( P == 2 )
        return solve2 ( v, A, B, C );

    if ( flag )
        v.clear ();

    int pw = Pow ( P, C );
    B %= pw;
    // Case 1:
    if ( B == 0 ) {
        if ( !flag )
            return Pow ( P, C - ( ( C + A - 1 ) / A ) );
        int base = Pow ( P, ( ( C + A - 1 ) / A ) ); //第一个可行解
        for ( int i = 0; i < pw; i += base )
            v.push_back ( i );
        return (int)v.size ();
    }
    // gcd ( B, pw ) = p^Q;
    int g = gcd ( B, pw ), Q = 0;
    B /= g;
    // Case 2 -> Case 3:
    while ( g > 1 )
        g /= P, Q++;
    if ( !flag )
        return Pow ( P, Q - Q / A ) * ( ( Q % A ) ? 0 : RHD ( v, A, B, P, C - Q, 0 ) );
    if ( Q % A )
        return 0;
    // Case 3:
    RHD ( tmp, A, B, P, C - Q, 1 );
    int base = Pow ( P, C - Q + Q / A ), tg = Pow ( P, Q / A ); //乘上tg才是真正的解
    for ( vector<int>::iterator i = tmp.begin (); i != tmp.end (); ++i )
        for ( int j = 1ll * ( *i ) * tg % base; j < pw; j += base ) //每隔base就有一个解
            v.push_back ( j );
    return (int)v.size ();
}

//中国剩余定理 a = ( a1c1 + a2c2 +... ) % P;
// res[ d ]存ad可能的值
vector<int> res[ 50 ];
set<int> Ans;
int crtk[ 50 ];
void dfsCRT ( int x, int d, int dcnt, int *p, int *q, int Fac ) {
    if ( d > dcnt ) {
        Ans.insert ( x );
        return;
    }
    // dfs,当前方程所有解当中选一个解出来建立方程组
    for ( int j = 0; j < (int)res[ d ].size (); ++j )
        dfsCRT ( ( 1ll * res[ d ][ j ] * crtk[ d ] + x ) % Fac, d + 1, dcnt, p, q, Fac );
}

//输入用
int read () {
    int x = 0;
    char ch = getchar ();
    while ( !isdigit ( ch ) )
        ch = getchar ();
    while ( isdigit ( ch ) )
        x = ( x << 1 ) + ( x << 3 ) + ( ch ^ 48 ), ch = getchar ();
    return x;
}

int main () {
#ifdef LOCAL
    freopen ( "in", "r", stdin );
    // freopen("out","w",stdout);
#endif
    Get_Prime ( 1e5 );
    int T = read ();
    while ( T-- ) {
        // INPt
        // x^A = B mod P
        int A = read (), P = read (), B = read ();

        int cnt, p[ 50 ], q[ 50 ];
        Prime_Decomposite ( P, p, q, cnt );
        swap ( p[ 1 ], p[ cnt ] );
        swap ( q[ 1 ], q[ cnt ] );

        //统计数量
        int ans = 1;
        for ( int i = 1; i <= cnt && ans > 0; ++i )
            ans *= solve ( res[ i ], A, B, p[ i ], q[ i ], 0 );
        if ( !ans ) {
            puts ( "No Solution" );
            continue;
        }
        //求每一个素因子的解
        for ( int i = 1; i <= cnt; ++i )
            solve ( res[ i ], A, B, p[ i ], q[ i ], 1 );

        // CRT解同余方程
        for ( int i = 1; i <= cnt; ++i ) {
            int v1 = Pow ( p[ i ], q[ i ] ), v2 = P / v1, x, y;
            exgcd ( v2, v1, x, y ); //求v2逆元
            x = ( x % v1 + v1 ) % v1;
            crtk[ i ] = 1ll * v2 * x % P; // a = ( a1c1 + a2c2 +... ) % P;
        }
        Ans.clear ();
        dfsCRT ( 0, 1, cnt, p, q, P );
        for ( set<int>::iterator i = Ans.begin (); i != Ans.end (); ++i )
            printf ( "%d ", *i );
        puts ( "" );
    }
    return 0;
}
