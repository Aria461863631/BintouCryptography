#include "EC256.cpp"
#include <fstream>

void Singular () {
    for ( int i = 0; i < 256; ++i )
        for ( int j = 0; j < 256; ++j ) {
            GF256 a1 = GF256 ( 1 );
            GF256 a2 = GF256 ( i );
            GF256 a6 = GF256 ( j );

            GF256 b2 = GF256 ( 1 );
            GF256 b6 = GF256 ( 0 );
            GF256 b8 = ( a1 * a1 * a6 );
            GF256 discriminate = ( b2 * b2 * b8 ) + ( b6 * b6 );

            if ( discriminate == GF256 ( 0 ) ) {
                cout << a2 << " , " << a6 << endl;
            }
        }
}

void correct_order () {
    for ( int i = 0; i < 256; ++i )
        for ( int j = 1; j < 256; ++j ) {
            EllipticCurve256 E ( i, j );
            int a = E.cardinality_bsgs ();
            int b = E.cardinality_brute_force ();
            cout << a << " " << b << endl;
            if ( a != b ) {
                cout << "false :\n";
            }
        }
}

void correct_generator () {
    // ofstream fout ( "file" );

    for ( int i = 0; i < 256; ++i )
        for ( int j = 1; j < 256; ++j ) {
            EllipticCurve256 E ( i, j );
            cout << "a&b :" << i << " " << j << endl;
            //       fout << "i&j :" << i << " " << j << endl;
            EC256 P = E.abelian_group ();
        }
    // fout.close ();
}

int main () {
    int a = 0, b = 1;
    EllipticCurve256 E = EllipticCurve256 ( a, b );

    /*
     * point plus
     */
    int x1, y1, x2, y2;
    x1 = 2, y1 = 48, x2 = 25, y2 = 3;
    EC256 P = EC256 ( x1, y1, a, b );
    EC256 Q = EC256 ( x2, y2, a, b );
    EC256 sol = P - Q;
    cout << sol << endl;

    return 0;
}
