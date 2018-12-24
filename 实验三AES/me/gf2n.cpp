#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "galois.h"

static int prim_poly[ 33 ] = {
    0,
    /*  1 */ 1,
    /*  2 */ 07,
    /*  3 */ 013,
    /*  4 */ 023,
    /*  5 */ 045,
    /*  6 */ 0103,
    /*  7 */ 0211,
    /*  8 */ 0435,
    /*  9 */ 01021,
    /* 10 */ 02011,
    /* 11 */ 04005,
    /* 12 */ 010123,
    /* 13 */ 020033,
    /* 14 */ 042103,
    /* 15 */ 0100003,
    /* 16 */ 0210013,
    /* 17 */ 0400011,
    /* 18 */ 01000201,
    /* 19 */ 02000047,
    /* 20 */ 04000011,
    /* 21 */ 010000005,
    /* 22 */ 020000003,
    /* 23 */ 040000041,
    /* 24 */ 0100000207,
    /* 25 */ 0200000011,
    /* 26 */ 0400000107,
    /* 27 */ 01000000047,
    /* 28 */ 02000000011,
    /* 29 */ 04000000005,
    /* 30 */ 010040000007,
    /* 31 */ // 020000000011,
    /* 32 */ // 00020000007
};

static int nw[ 33 ] = {0,           ( 1 << 1 ),  ( 1 << 2 ),  ( 1 << 3 ),  ( 1 << 4 ),  ( 1 << 5 ),
                       ( 1 << 6 ),  ( 1 << 7 ),  ( 1 << 8 ),  ( 1 << 9 ),  ( 1 << 10 ), ( 1 << 11 ),
                       ( 1 << 12 ), ( 1 << 13 ), ( 1 << 14 ), ( 1 << 15 ), ( 1 << 16 ), ( 1 << 17 ),
                       ( 1 << 18 ), ( 1 << 19 ), ( 1 << 20 ), ( 1 << 21 ), ( 1 << 22 ), ( 1 << 23 ),
                       ( 1 << 24 ), ( 1 << 25 ), ( 1 << 26 ), ( 1 << 27 ), ( 1 << 28 ), ( 1 << 29 ),
                       ( 1 << 30 ), ( 1 << 31 ), -1};

static int *Log = NULL, *ALog = NULL;

/*
 *移位乘法
 */
int galois_shift_multiply ( int x, int y, int w ) {
    if ( x == 0 || y == 0 )
        return 0;

    int res = 0;
    while ( y ) {
        if ( y & 1 )
            res = res ^ x;
        x = ( x << 1 ) ^ ( x & nw[ w - 1 ] ? prim_poly[ w ] : 0 );
        y >>= 1;
    }
    return res;
}

/*
 * 打表
 * 所有生成元都是2
 */
int galois_create_log_table ( int w ) {
    if ( w > 30 )
        return -1;
    if ( Log ) {
        delete[] Log;
        delete[] ALog;
    }

    Log = new int[ nw[ w ] ];
    if ( !Log )
        return -1;
    ALog = new int[ nw[ w ] ];
    if ( !ALog ) {
        delete[] Log;
        return -1;
    }

    int g = 1;
    for ( int j = 0; j < nw[ w ] - 1; ++j ) {
        Log[ g ] = j;
        ALog[ j ] = g;

        g = g << 1; // g == 2
        if ( g & nw[ w ] )
            g = ( g ^ prim_poly[ w ] ) & ( nw[ w ] - 1 );
    }
    return 0;
}

int galois_logtable_multiply ( int x, int y, int w ) {
    if ( galois_create_log_table ( w ) == -1 )
        return -1;
    if ( x == 0 || y == 0 )
        return 0;
    int sum_j = ( Log[ x ] + Log[ y ] ) % ( nw[ w ] - 1 );
    int sol = ALog[ sum_j ];
    return sol;
}

int galois_logtable_divide ( int x, int y, int w ) {
    if ( galois_create_log_table ( w ) == -1 )
        return -1;
    if ( y == 0 )
        return -1;
    if ( x == 0 )
        return 0;
    int sum_j = ( Log[ x ] - Log[ y ] ) % ( nw[ w ] - 1 );
    int sol = ALog[ sum_j ];
    return sol;
}

int main () {
#ifdef LOCAL
    freopen ( "in", "r", stdin );
    // freopen("out","w",stdout);
#endif
    int x, y, w;
    printf ( "%d\n", prim_poly[ 16 ] );
    while ( ~scanf ( "%d%d%d", &x, &y, &w ) ) {
        if ( w > 22 )
            printf ( "%d\n", galois_shift_multiply ( x, y, w ) );
        else
            printf ( "%d\n", galois_logtable_multiply ( x, y, w ) );
    }
    return 0;
}
