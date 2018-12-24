#include "Numtheory.cpp"
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

/*
 * arithmetic on GF(2^8)
 */
class GF256 {
            public:
    GF256 () {}
    GF256 ( int v )
        : value ( v % 256 ) {}

    int get () { return value; }

    friend GF256 operator+ ( GF256 a, GF256 b ) { return GF256 ( a.value ^ b.value ); }

    friend GF256 operator- ( GF256 a, GF256 b ) { return GF256 ( a.value ^ b.value ); }

    friend GF256 operator* ( GF256 a, GF256 b ) { return GF256 ( Multiply ( a.value, b.value ) ); }

    friend GF256 operator/ ( GF256 a, GF256 b ) {
        if ( b.value == 0 )
            cout << "ERROR: Divide zero" << endl;
        return GF256 ( Multiply ( a.value, Inverse ( b.value ) ) );
    }

    friend bool operator== ( GF256 a, GF256 b ) { return ( a.value == b.value ); }

    friend bool operator< ( GF256 a, GF256 b ) { return a.value < b.value; }

    friend inline ostream &operator<< ( ostream &os, GF256 &a ) {
        if ( a.value == 0 ) {
            cout << 0;
            return os;
        }
        int s = a.value, i = 7;
        while ( i >= 0 ) {
            if ( ( s >> i ) & 1 ) {
                if ( i == 0 )
                    cout << "1";
                else if ( s % ( 1 << i ) == 0 )
                    i == 1 ? cout << "a" : cout << "a^" << i;
                else if ( i == 1 )
                    cout << "a + ";
                else
                    cout << "a^" << i << " + ";
            }
            --i;
        }
        return os;
    }

            private:
    int value;

    static int Divide ( int x, int y, int &tmp ) {
        int result = 0;
        int y_High = GetHigh ( y );
        char diff = GetHigh ( x ) - y_High;
        if ( diff < 0 ) {
            tmp = x;
            return 0;
        }

        while ( diff >= 0 ) {
            result |= ( 1 << diff );
            x = ( x ^ ( y << diff ) );
            diff = GetHigh ( x ) - y_High;
        }

        tmp = x;
        return result;
    }

    static int Multiply ( int a, int b ) {
        int res = 0;
        while ( b ) {
            if ( b & 1 )
                res = res ^ a;
            a = ( a << 1 ) ^ ( a & 0x80 ? 0x11d : 0 );
            b >>= 1;
        }
        return res;
    }

    static int Inverse ( int y ) {
        int a, b;
        Egcd ( 285, y, a, b );
        return b;
    }

    // Get the highest bit of the polynomial
    static char GetHigh ( int num ) {
        for ( char i = 15; i >= 0; --i ) {
            if ( num >> i )
                return i;
        }
        return -1;
    }

    // Polynomial mod 0x11B
    static int GF_Mod ( int x ) { return ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) ); }

    static int Egcd ( int a, int b, int &x, int &y ) {
        if ( b == 0 ) {
            x = 1;
            y = 0;
            return a;
        }
        int tmp;
        int divide = Divide ( a, b, tmp );
        int gcd = Egcd ( b, tmp, y, x );
        y ^= Multiply ( divide, x );
        // printf ( "egcd: a:%d b:%d x:%d y:%d q:%d\n", a, b, x, y, divide );
        return gcd;
    }
};

/*
 * Arithmetic of points on elliptic curve
 */
class EC256 {
            public:
    EC256 () {
        x = GF256 ( 0 ), y = GF256 ( 0 );
        A = 1, B = 1;
    }
    EC256 ( int a, int b, GF256 _A, GF256 _B )
        : x ( GF256 ( a % 256 ) )
        , y ( GF256 ( b % 256 ) )
        , A ( _A )
        , B ( _B ) {}

    friend bool operator== ( EC256 a, EC256 b ) {
        return ( a.x.get () == b.x.get () && a.y.get () == b.y.get () );
    }

    friend EC256 operator+ ( EC256 a, EC256 b ) {
        if ( ( a.A == b.A ) && ( a.B == b.B ) ) {

            if ( a.is_zero () )
                return b;
            if ( b.is_zero () )
                return a;

            if ( a == b ) {
                if ( a.x == GF256 ( 0 ) )
                    return EC256 ( 0, 0, a.A, a.B );
                GF256 lambda = a.x + ( a.y / a.x );

                GF256 xr = lambda * lambda + lambda + a.A;

                GF256 yr = ( a.x * a.x ) + lambda * xr + xr;

                return EC256 ( xr.get (), yr.get (), a.A, a.B );
            }

            else {
                GF256 lambda = ( a.x + b.x );
                if ( lambda == GF256 ( 0 ) )
                    return ( EC256 ( 0, 0, a.A, a.B ) );
                lambda = ( a.y + b.y ) / ( a.x + b.x );

                GF256 xr = lambda * lambda + lambda + a.x + b.x + a.A;

                GF256 yr = lambda * ( a.x + xr ) + xr + a.y;

                return EC256 ( xr.get (), yr.get (), a.A, a.B );
            }
        } else
            return EC256 ( 0, 0, a.A, a.B );
    }

    friend EC256 operator- ( EC256 a, EC256 b ) {
        if ( a.A == b.A && a.B == b.B ) {
            if ( b.is_zero () )
                return a;
            b = EC256 ( b.x.get (), ( b.x + b.y ).get (), a.A, a.B );
            return a + b;
        } else
            return EC256 ( 0, 0, a.A, a.B );
    }

    friend EC256 operator* ( int n, EC256 a ) {
        if ( n == 0 )
            return EC256 ( 0, 0, a.A, a.B );
        if ( n == 2 )
            return a + a;
        if ( n < 0 )
            return -n * EC256 ( a.x.get (), ( a.x + a.y ).get (), a.A, a.B );
        n--;
        EC256 res = EC256 ( a.x.get (), a.y.get (), a.A, a.B );
        while ( n ) {
            if ( n & 1 )
                res = res + a;
            n >>= 1;
            a = 2 * a;
        }
        return res;
    }

    friend bool operator< ( EC256 a, EC256 b ) { return a.x < b.x; }

    friend inline ostream &operator<< ( ostream &os, EC256 &a ) {
        cout << "E( " << a.x << " , " << a.y << " ) ";
        return os;
    }

    bool is_zero () {
        if ( x == 0 && y == 0 )
            return true;
        return false;
    }

            private:
    // y^2 + xy = x^3 + Ax^2 + B
    GF256 x, y;

            protected:
    GF256 A, B;
};

class EllipticCurve256 : public EC256 {
            public:
    // y^2 + xy = x^3 + Ax^2 +B;
    EllipticCurve256 ( int a1, int b1 )
        : EC256 ( 0, 0, GF256 ( a1 ), GF256 ( b1 ) ) {
        if ( b1 == 0 )
            cout << "ERROR: Singular Elliptic Curve" << endl;
        srand ( unsigned( time ( 0 ) ) );
        order = -1;
    }

    /*
     * using brute force to get order of the group
     */
    int cardinality_brute_force () {
        generate_all_points ();

        sort ( points.begin (), points.end () );
        return order;
    }

    /*
     * find all points using brute force
     */
    void all_points () {
        generate_all_points ();

        sort ( points.begin (), points.end () );
        for ( int i = 0; i < (int)points.size (); ++i )
            cout << points[ i ] << endl;
    }

    /*
     * find all points according one x, return one
     */
    EC256 lift_x ( GF256 x ) {
        if ( mmp[ 0 ].size () == 0 )
            generate_map ();

        // y^2 + xy = f
        GF256 f = ( ( x + A ) * x * x ) + B;

        //(y/x)^2 + (y/x) +( f/x^2 ) = 0
        f = f / ( x * x );
        vector<GF256> roots = mmp[ f.get () ];

        // could have no solution according to x
        if ( roots.size () == 0 )
            return O;

        OK = false;
        for ( int i = 0; i < (int)roots.size (); ++i )
            roots[ i ] = roots[ i ] * x;

        // randomly choose a point to return
        int y_int = rand () % (int)roots.size ();

        return EC256 ( x.get (), roots[ y_int ].get (), A, B );
    }

    /*
     * randomly choose a point on the curve
     */
    EC256 random_element () {
        OK = true;
        EC256 sol;
        while ( OK ) {
            int x_int = rand () % 255 + 1;

            GF256 x = GF256 ( x_int );
            sol = lift_x ( x );
        }
        return sol;
    }

    /*
     * randomly choose a point on the curve using brute force
     */
    EC256 random_point () {
        if ( points.size () == 0 )
            generate_all_points ();
        int ind = rand () % points.size ();
        cout << points[ ind ] << endl;
        return points[ ind ];
    }

    /*
     * BSGS, return the order of a point
     */
    int pointorder_bsgs ( EC256 P ) {

        /*
         * 1.
         * Compute Q = (q + 1)P
         */
        const int q = 256;
        EC256 Q = ( q + 1 ) * P;

        // cout << "Q :" << Q << endl;

        /*
         * 2.
         * Choose an integer m with m > q^1/4 .
         * Compute and store the points jP
         * for j = 0, 1, 2, . . . , m.
         */
        const int m = 5;

        EC256 jP[ 6 ];
        for ( int j = 0; j <= m; ++j )
            jP[ j ] = j * P;
        // cout << "jpi :" << jP[ 3 ] << endl;

        EC256 mP = EC256 ( P );
        mP = ( 2 * m ) * mP;
        // cout << "mP :" << mP << endl;

        EC256 Q2 = EC256 ( Q );

        /*
         * 3.
         * Compute the points
         * Q + k(2mP) for k = −m, −(m − 1), . . . , m
         * until there is a match Q + k(2mP) = ±jP
         */
        int M = 0;
        bool fg = false;
        for ( int k = -m; k <= m; ++k ) {
            Q2 = Q + k * mP;
            for ( int j = 0; j <= m; ++j ) {
                if ( Q2 == jP[ j ] ) {
                    M = q + 1 + 2 * m * k - j;

                    fg = 1;
                    break;
                } else if ( Q2 == ( -1 * jP[ j ] ) ) {
                    M = q + 1 + 2 * m * k + j;

                    fg = 1;
                    break;
                }
            }
            if ( fg )
                break;
        }
        /*
         * 4.
         * Conclude that (q + 1 + 2mk ∓ j)P = ∞.
         * Let M = q + 1 + 2mk ∓ j
         */
        // dbg ( "pointorder_bsgs M ", M );

        int factors[ 100 ][ 2 ];
        while ( 1 ) {
            fg = 0;
            /*
             * 5.
             * Factor M . Let p1 , . . . , pr
             * be the distinct prime factors of M .
             */
            int faCnt = getFactors ( M, factors );

            /*
             * 6.1
             * Compute (M/pi)P for i = 1, . . . , r.
             * If (M/pi)P = ∞ for some i, replace
             * M with M/pi and go back to step (5)
             */
            for ( int i = 0; i < faCnt; ++i )
                if ( ( M / factors[ i ][ 0 ] ) * P == O ) {
                    M = M / factors[ i ][ 0 ];
                    fg = 1;
                    break;
                }

            /*
             * 6.2
             * If (M/pi)P = ∞ for all i then
             * M is the order of the point P
             */
            if ( fg == 0 )
                return M;
        }
        return -1;
    }

    /*
     * return the order of the group using bsgs
     */
    int cardinality_bsgs () {
        if ( order != -1 )
            return order;

        dbg ( "\n---- bsgs begin -----" );

        // Hasse bound
        const int q = 256;
        int lower_bound = q + 1 - 32;
        int upper_bound = q + 1 + 32;

        order = 1;
        bool fg = true;
        int level = 1;
        while ( fg ) {
            dbg ( "-- level ", level++, " --" );

            EC256 P = random_element ();
            // cout << "P: " << P << endl;
            int n1 = pointorder_bsgs ( P );

            order = lcm ( order, n1 );
            // cout << "lcmorder " << order << endl;

            int c = unique_in_bound ( order, lower_bound, upper_bound );
            // cout << "c " << c << endl;
            if ( c < 2 ) {
                order = order * ( upper_bound / order );
                fg = false;
            }
        }
        dbg ( "Group order ", order );
        dbg ( "---- bsgs end -----\n" );
        return order;
    }

    /*
     * merge two points to extend the order of the point
     */
    EC256 merge_points ( EC256 P, EC256 Q, int n1, int n2, int &ord ) {
        // trivial case
        if ( n2 % n1 == 0 ) {
            ord = n2;
            return Q;
        }
        if ( n1 % n2 == 0 ) {
            ord = n1;
            return P;
        }

        int n = lcm ( n1, n2 );
        int m1 = 1, m2 = n2 / ( n / n1 );
        P = m1 * P;
        Q = m2 * Q;
        ord = n;
        dbg ( "merge point order ", ord );

        return P + Q;
    }

    /*
     * find a generator of the group
     */
    EC256 abelian_group () {
        dbg ( "\n---- abelian_group begin -----" );

        if ( order == -1 )
            cardinality_bsgs ();

        EC256 P1 = O;
        EC256 P2 = O;
        int n1 = 1;
        int n2 = 1;
        int npts = 0;

        while ( n1 * n2 != order ) {
            EC256 Q = random_element ();

            while ( Q.is_zero () )
                Q = random_element ();
            npts++;

            dbg ( "Random point Q ", Q );
            EC256 Q1 = n1 * Q;

            /*
             * E is not a cyclic group
             */
            if ( Q1.is_zero () && npts >= 20 ) {

                dbg ( "Case 2: n2 may increase" );

                int n1a = 1, n1b = n1;
                EC256 P1a = P1;
                n1a = prime_to_m_part ( n1, order / n1 );
                dbg ( "n1a = ", n1a );
                n1b = n1 / n1a;
                dbg ( "n1b = ", n1b );

                Q = n1a * Q; // has order | n1b
                dbg ( "Q = ", Q );
                P1a = n1a * P1; // has order = n1b
                dbg ( "P1a = ", P1a );

                int di[ 100 ];
                int a = -1, cnt = getDivisors ( n1b, di ), indx = 0, mm;
                dbg ( "cnt ", cnt );
                while ( a == -1 && indx < cnt ) {
                    mm = di[ indx ];
                    a = generic_bsgs ( mm * P1a, mm * Q, 0, n1b / mm - 1 );
                    dbg ( "a", a );
                    ++indx;
                }
                if ( a == -1 ) {
                    cout << "a == -1" << endl;
                    continue;
                }

                a *= ( mm * n1a );
                dbg ( "linear relation aP1 = mQ where a is ", a, " and m is ", mm );
                if ( mm > 1 ) { // if Q is not in <P1>
                    Q = Q - ( a / mm ) * P1;
                    dbg ( "Q again: ", Q );
                    int o = pointorder_bsgs ( Q );
                    dbg ( "Q.order == m ? ", o, mm );

                    if ( n2 == 1 ) { // first time
                        P2 = Q;
                        n2 = mm;

                        dbg ( "Adding second generator ", P2, "of order ", n2 );
                        dbg ( "Subgroup order now ", n1 * n2 );
                    } else { // extend
                        int oldn2 = n2;
                        P2 = merge_points ( P2, Q, n2, mm, n2 );

                        if ( n2 / oldn2 > 1 ) {
                            dbg ( "Replacing second generator by ", P2 );
                            dbg ( "gaining index ", n2 / oldn2 );
                            dbg ( "Subgroup order is now:", n1 * n2 );
                        }
                    }
                }
            }
            /*
             * E might be a cyclic group
             */
            else {
                int oldn1 = n1;
                int m = pointorder_bsgs ( Q );
                P1 = merge_points ( P1, Q, n1, m, n1 );

                if ( n1 / oldn1 > 1 ) {
                    dbg ( "Replacing generator by ", P1 );
                    dbg ( "gaining index ", n1 / oldn1 );
                    dbg ( "Subgroup order is now:", n1 * n2 );
                }
            }
        }
        /*
        if ( n2 > 1 )
            cout << "Non cyclic group isomorphic to Z" << n1 << " + Z" << n2 << endl;
        cout << "Generator P1 = " << P1 << endl;
        if ( n2 > 1 )
            cout << "P2 = " << P2 << endl;
        */
        if ( n2 > 1 )
            cout << "NO" << endl;
        else
            cout << "YES" << endl;

        dbg ( "---- abelian_group end -----\n" );
        return P1;
    }

            private:
    int order;
    vector<EC256> points;
    const EC256 O = EC256 ( 0, 0, A, B );
    vector<GF256> mmp[ 260 ];
    GF256 rhs[ 300 ], lhs[ 300 ];
    bool OK;

    void generate_all_points () {
        points.push_back ( O ); // INF
        for ( int i = 0; i < 256; ++i ) {
            GF256 x = GF256 ( i );
            rhs[ i ] = ( x * x * x ) + ( A * x * x ) + B;
            lhs[ i ] = x * x;
        }
        //暴力遍历所有可能的解
        for ( int i = 0; i < 256; ++i ) {
            for ( int j = 0; j < 256; ++j )
                if ( lhs[ i ] + GF256 ( j ) * GF256 ( i ) == rhs[ j ] ) {
                    points.push_back ( EC256 ( j, i, A, B ) );
                }
        }
        order = points.size ();
    }

    /*
     * map for x^2+x
     */
    void generate_map () {
        for ( int i = 0; i < 256; ++i ) {
            GF256 x = GF256 ( i );
            x = x * x + x;

            mmp[ x.get () ].push_back ( GF256 ( i ) );
        }
    }

    /*
     * whether an order has unqiue multiple in Hasse bound
     */
    int unique_in_bound ( int n, int lb, int ub ) {
        int cl = ceil ( lb / n );
        int cu = ub / n;

        return cu - cl;
    }

    /*
     * return k where a = kb and lb <= k <= ub
     */
    int generic_bsgs ( EC256 a, EC256 b, int lb, int ub ) {
        dbg ( "-- generic bsgs --" );
        dbg ( "a ", a );
        dbg ( "b ", b );

        for ( int i = lb; i <= ub; ++i )
            if ( i * a == b )
                return i;
        return -1;
    }
};
