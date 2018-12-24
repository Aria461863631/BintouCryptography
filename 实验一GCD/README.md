### 1. 请给出GCD算法的迭代版本。

```python
def gcd_iter(a, b):
    if a < b:
        a, b = b, a
    while b:
        tmp = a % b
        a = b
        b = tmp
    return a
```



### 2. 根据老师给出的代码（或者自己写代码）验证EGCD算法的正确性。

$$
\begin{aligned} 
ax + by & = gcd(b, a \% b) \\ 
& = bx^{'} + (a\%b)y^{'} \\
& = bx^{'} + ( a - \lfloor\frac{a}{b}\rfloor b)y^{'} \\
& = bx^{'} + ay^{'} - \lfloor\frac{a}{b}\rfloor by^{'} \\
& = ay^{'} + b( x^{'} - \lfloor\frac{a}{b}\rfloor y^{'})
\end{aligned}
$$

so $ x = y^{'} $ and $ y =  x^{'} - \lfloor\frac{a}{b}\rfloor y^{'} $

```python
# solve ax + by = gcd(a,b)
def exgcd_matrix(a, b):
    '''
     r0, s0
     r1, s1
    '''
    # init: a + 0*b = a; 0*a + b = b;
    r0, r1, s0, s1 = 1, 0, 0, 1
    # row1 - row2 * ( a/b * b )
    # therefore a - ( a/b * b ) = a % b
    # swap row1 and row2 as gcd
    while b:
        q, a, b = a//b, b, a % b
        r0, r1 = r1, r0 - q * r1
        s0, s1 = s1, s0 - q * s1
    return a, r0, s0


def exgcd_recr(a, b):
    if b == 0:
        return a, 1, 0
    else:
        d0, x0, y0 = exgcd_recr(b, a % b)
        d, x, y = d0, y0, x0 - a//b * y0
        return d, x, y

```



### 3. 编程实现同余式的求解。给定同余式 ax = b (mod N)，即给定整数a、b和N，求x。如果无解，也要给出“无解”的提示。

```python
# solve ax = b (mod n)
# first solve ax + ny = gcd(a,n)
# if d = gcd(a,n) | b, solvable with d solutions
def modular_linear_equation_solver(a, b, n):
    g, y, x = exgcd_recr(n, a)
    if b % g == 0:
        x0 = x * b//g % n
        for i in range(g):
            print((x0 + i * n//g)%n, end=' ')
    else:
        print("unsolvable")
```



### 4. 编程实现求乘法逆元。给定同余式 ax = (1 mod N)，即给定整数a和N，求x。如果无解，也要给出“无解”的提示。

```python
def quick_pow(a, n):
    ans = 1
    while n:
        if n & 1:
            ans *= a
        a *= a
        n >>= 1
    return ans


# solve ax = 1 (mod n) (2 methods)
# gcd(a,n)==1, solve ax + ny = 1
def inverse (a, n):
    g, y, x = exgcd_recr(n, a)
    if g == 1:
        # exgcd
        print(x % n)
        # fermat little theorem
        print(quick_pow(a, n-2) % n)
    else:
        print("unsolvable")
```



### 5. 编程实现开任意次方。给定同余式 x^a = b (mod N)，求x。即给定a、x的a次方（b）、N，求出x。如果无解，也要给出“无解”的提示。提示：欧拉定理。

//bzoj2219 51nod1123

求解 $ x^A = B ( mod \  P ) $ P为任意数

1. 根据**中国剩余定理:** $ a  \leftrightarrow  ( a_1, a_2, .. a_k ) $ where $ a_i = a \ mod \ p_i $ 有双射关系

   则可以把P分解成$p_1^{c1}p_2^{c2}...p_k^{ck} $

   对每个$p_i^{ci} $ 求解 $ x_i ^{A} = B ( mod \ p_i^{ci}) $

   最后在用中国剩余定理求解同余方程组的解,即模P下的解

**于是问题变成了求 $ x^A≡B(mod \ p^c) $的解**

1. p为偶数,即p有 $ 2^k $ 为因子

   - 考虑到 $ x^A≡B(mod \ 2^k) $ 的充要条件是 $ x^A≡B(mod \ 2^{k−1}) $ 。
   - 反过来，如果 $ x^A≡B(mod \ 2^{i−1}) $，那么只可能导致  $ x^A≡B(mod \ 2^i) $ 或 $ (x+2i−1)^A≡B(mod\ 2^i) $
   - dfs测试两种情况哪一种满足

2. p为奇数

   - **Case 1:** 
     $$
     gcd ( p^c, B ) = p^c
     $$
     即$ x^A≡0(mod \ p^c) $ 

     立刻得到第一个解 $ x = p^{⌈\frac{c}{A}⌉}$  (保证$ ⌈\frac{c}{A}⌉ ∗A≥a $ )

     接着从 $ p^{⌈\frac{c}{A} + 1⌉} ,  p^{⌈\frac{c}{A} + 2⌉} ...  $一直到 $ p^c $ 都可以是解

   - **Case 2:** 
     $$
     gcd ( p^c, B ) = p^Q
     $$
     即有相同的素因子

     设B为$ p^Q * b $ ,原方程变为
     $$
     x^A≡p^Q∗b(mod \ p^c)
     $$
     如果$ Q \ mod \  A  \neq  0 $ ,无解

     否则变成下面的式子, 转换为第三种情况
     $$
     \left(\frac{x}{p^{\frac{Q}{A}}}\right)^A\equiv b\pmod{p^{c-Q}}
     $$
     得到 $ \left(\frac{x}{p^{\frac{Q}{A}}}\right) $ 后乘上 $p^{\frac{Q}{A}}$就是解, 但因为解上面式子的时候x在 $ \pmod{p^{c-Q+{\frac{Q}{A}}}} $的范围下,而我们x

     本应该在 $ \pmod{ p^{c-Q}} $ 的范围下,所以对每个解不断加上 $  {p^{c-Q+{\frac{Q}{A}}}}  $ 来求出所有的解

   - **Case 3:**
     $$
     gcd ( p^c, B ) = 1
     $$
     方程为 $  x^A≡B(mod \ p^c) $

     利用指标 : 对生成元g, 另 $ B = g^t $, 则t是B的指标,记作ind(B)

     两边对g取对数, 方程转化为
     $$
     A*ind(x)\equiv ind(B)\pmod{\varphi(p^c)}
     $$
     先求出生成元g,通过离散对数的BSGS算法求出B的指标ind(B),则已知的数有$ A,ind(B),\varphi(p^c) $

     方程的形式与ax=b(mod n)对应
     $$
     ax\equiv b\pmod n
     $$
     求解得到 0 或 $ gcd ( a, n ) $ 个ind(x), 则$ g^{ind(x)} $就是解

**代码**

```c++
//51nod1123
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
```



### 6. （开放型作业，可选做）给定一个整数序列x[0]、x[1]...、x[n]，设计一个算法求出所有x[i]与x[j]的最大公因子（i != j）。

提示，x[i]可能很大，比如1024比特。n可以比较小，比如，1000之内的规模。所以，答案应该是一个整数序列。可以用Python做，用C/C++可以考虑GMP。



batchgcd

- gcd{*X*[0],*X*[1]*X*[2]*X*[3]...};
- gcd{*X*[1],*X*[0]*X*[2]*X*[3]...};
- gcd{*X*[2],*X*[0]*X*[1]*X*[3]...};
- etc.

求出batchgcd之后, 用求出来的d再与其他数取gcd,更快

```python
def product(X):
    if len(X) == 0:
        return 1
    while len(X) > 1:
       X = [np.prod(X[i*2:(i+1)*2]) for i in range((len(X)+1)//2)]
    return X[0]


def producttree(X):
       result = [X]
       while len(X) > 1:
         X = [np.prod(X[i*2:(i+1)*2]) for i in range((len(X)+1)//2)]
         result.append(X)
       return result


def remaindersusingproducttree(n, T):
    result = [n]
    for t in reversed(T):
        result = [result[math.floor(i/2)] % t[i] for i in range(len(t))]
    return result


def remainders(n, X):
    return remaindersusingproducttree(n, producttree(X))


def batchgcd_simple(X):
    R = remainders(product(X), [n**2 for n in X])
    return [math.gcd(r//n, n) for r, n in zip(R, X)]


def all_pairs_gcd(X):
    D = batchgcd_simple(X)
    l = []
    for i in range(len(X)):
        l.append([math.gcd(D[i], X[j]) for j in range(len(X))])
    return l


print(all_pairs_gcd([1, 2, 3, 4, 5, 6, 7]))
```



**老哥们找到的参考连接**

师兄说

- batchgcd计算的时候计算,X[0]X[1]..X[i-1]X[i+1]..X[n] mod X[i]^2 的时候
- 乘积可能很大,所以并不是先全部乘起来再取模的,而是一边取模一边乘的

dlm

- java实现的batchgcd https://github.com/Alexis-D/batch-gcd

- Factoring RSA Moduli. Part I. https://windowsontheory.org/2012/05/15/979
- RSA factorization in the real world https://www.hyperelliptic.org/tanja/vortraege/facthacks-29C3.pdf

小明

- finding kth root mod n http://www.oxfordmathcenter.com/drupal7/node/179