#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "sha3.h"

#define SHA3_CONST( x ) x##L
#define SHA3_ASSERT( x )
#define SHA3_TRACE( format, args... )
#define SHA3_TRACE_BUF( format, buf, l, args... )

#define SHA3_ROTL64( x, y ) \
    ( ( ( x ) << ( y ) ) | ( ( x ) >> ( ( sizeof ( uint64_t ) * 8 ) - ( y ) ) ) )

static const uint64_t keccakf_rndc[ 24 ] = {
    SHA3_CONST ( 0x0000000000000001UL ), SHA3_CONST ( 0x0000000000008082UL ),
    SHA3_CONST ( 0x800000000000808aUL ), SHA3_CONST ( 0x8000000080008000UL ),
    SHA3_CONST ( 0x000000000000808bUL ), SHA3_CONST ( 0x0000000080000001UL ),
    SHA3_CONST ( 0x8000000080008081UL ), SHA3_CONST ( 0x8000000000008009UL ),
    SHA3_CONST ( 0x000000000000008aUL ), SHA3_CONST ( 0x0000000000000088UL ),
    SHA3_CONST ( 0x0000000080008009UL ), SHA3_CONST ( 0x000000008000000aUL ),
    SHA3_CONST ( 0x000000008000808bUL ), SHA3_CONST ( 0x800000000000008bUL ),
    SHA3_CONST ( 0x8000000000008089UL ), SHA3_CONST ( 0x8000000000008003UL ),
    SHA3_CONST ( 0x8000000000008002UL ), SHA3_CONST ( 0x8000000000000080UL ),
    SHA3_CONST ( 0x000000000000800aUL ), SHA3_CONST ( 0x800000008000000aUL ),
    SHA3_CONST ( 0x8000000080008081UL ), SHA3_CONST ( 0x8000000000008080UL ),
    SHA3_CONST ( 0x0000000080000001UL ), SHA3_CONST ( 0x8000000080008008UL )};

static const unsigned keccakf_rotc[ 24 ] = {1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
                                            27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44};

static const unsigned keccakf_piln[ 24 ] = {10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
                                            15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1};

static void keccakf ( uint64_t s[ 25 ] ) {
    uint64_t bc[ 5 ];
#define KECCAK_ROUNDS 24

    for ( int round = 0; round < KECCAK_ROUNDS; ++round ) {

        for ( int i = 0; i < 5; ++i )
            bc[ i ] = s[ i ] ^ s[ i + 5 ] ^ s[ i + 10 ] ^ s[ i + 15 ] ^ s[ i + 20 ];

        /* Theta */
        for ( int i = 0; i < 5; ++i ) {
            //(i+4)%5: col pre; (i+1)%5: col nex
            uint64_t t = bc[ ( i + 4 ) % 5 ] ^ SHA3_ROTL64 ( bc[ ( i + 1 ) % 5 ], 1 );
            for ( int j = 0; j < 25; j += 5 )
                s[ j + i ] ^= t;
        }

        /* Rho & Pi */
        uint64_t cor = s[ 1 ]; // start with coordinate(1,0)
        for ( int t = 0; t < 24; ++t ) {
            int nex = keccakf_piln[ t ]; // pi
            uint64_t tmp = s[ nex ];
            s[ nex ] = SHA3_ROTL64 ( cor, keccakf_rotc[ t ] ); // rho
            cor = tmp;
        }

        /* Chi */
        for ( int j = 0; j < 25; j += 5 ) { // j = {0, 5, 10, 15, 20}
            for ( int i = 0; i < 5; ++i )
                bc[ i ] = s[ j + i ];     // whole row
            for ( int i = 0; i < 5; ++i ) // nex1 and nex2 row
                s[ j + i ] ^= ( ~bc[ ( i + 1 ) % 5 ] ) & bc[ ( i + 2 ) % 5 ];
        }

        /* Iota */
        s[ 0 ] ^= keccakf_rndc[ round ];
    }
}

void sha3_Init256 ( void *priv ) {
    sha3_context *ctx = (sha3_context *)priv;
    memset ( ctx, 0, sizeof ( *ctx ) );
    ctx->capacityWords = 2 * 256 / ( 8 * sizeof ( uint64_t ) );
}

void sha3_Update ( void *priv, void const *bufIn, size_t len ) {
    sha3_context *ctx = (sha3_context *)priv;

    //还差多少组成一个word
    unsigned old_tail = ( 8 - ctx->byteIndex ) & 7;

    size_t words;
    unsigned tail;
    size_t i;

    const uint8_t *buf = bufIn;

    // update的长度不足一个word
    if ( len < old_tail ) {
        while ( len-- )
            ctx->saved |= ( uint64_t ) ( *( buf++ ) ) << ( ( ctx->byteIndex++ ) * 8 );
        return;
    }

    if ( old_tail ) {
        len -= old_tail;
        while ( old_tail-- )
            ctx->saved |= ( uint64_t ) ( *( buf++ ) ) << ( ( ctx->byteIndex++ ) * 8 );

        //先update一部分补全一个word
        ctx->s[ ctx->wordIndex ] ^= ctx->saved;

        //更新index
        ctx->byteIndex = 0;
        ctx->saved = 0;
        if ( ++ctx->wordIndex == ( SHA3_KECCAK_SPONGE_WORDS - ctx->capacityWords ) ) {
            keccakf ( ctx->s );
            ctx->wordIndex = 0;
        }
    }

    //处理剩下的满足word的部分
    words = len / sizeof ( uint64_t );
    tail = len - words * sizeof ( uint64_t );

    for ( int i = 0; i < words; ++i, buf += sizeof ( uint64_t ) ) {
        const uint64_t t =
            ( uint64_t ) ( buf[ 0 ] ) | ( ( uint64_t ) ( buf[ 1 ] ) << 8 * 1 ) |
            ( ( uint64_t ) ( buf[ 2 ] ) << 8 * 2 ) | ( ( uint64_t ) ( buf[ 3 ] ) << 8 * 3 ) |
            ( ( uint64_t ) ( buf[ 4 ] ) << 8 * 4 ) | ( ( uint64_t ) ( buf[ 5 ] ) << 8 * 5 ) |
            ( ( uint64_t ) ( buf[ 6 ] ) << 8 * 6 ) | ( ( uint64_t ) ( buf[ 7 ] ) << 8 * 7 );

        ctx->s[ ctx->wordIndex ] ^= t;
        if ( ++ctx->wordIndex == ( SHA3_KECCAK_SPONGE_WORDS - ctx->capacityWords ) ) {
            keccakf ( ctx->s );
            ctx->wordIndex = 0;
        }
    }

    /* save partial word */
    while ( tail-- ) {
        ctx->saved |= ( uint64_t ) ( *( buf++ ) ) << ( ( ctx->byteIndex++ ) * 8 );
    }
}

// padding: 10*1
void const *sha3_Finalize ( void *priv ) {

    sha3_context *ctx = (sha3_context *)priv;

    ctx->s[ ctx->wordIndex ] ^= ( ctx->saved ^ ( ( uint64_t ) ( ( uint64_t ) ( 0x02 | ( 1 << 2 ) )
                                                                << ( ( ctx->byteIndex ) * 8 ) ) ) );

    ctx->s[ SHA3_KECCAK_SPONGE_WORDS - ctx->capacityWords - 1 ] ^=
        SHA3_CONST ( 0x8000000000000000UL );
    keccakf ( ctx->s );

    return ( ctx->sb );
}
