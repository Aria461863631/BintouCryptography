#include <stddef.h>
#include <stdint.h>
#ifndef SHA3_H
#define SHA3_H

// default r + c = 1600bits;
// = 25words; sizeof(uint64_t) = 8bytes = 64bits;
#define SHA3_KECCAK_SPONGE_WORDS ( ( ( 1600 ) / 8 ) / sizeof ( uint64_t ) )

typedef struct sha3_context_ {
    uint64_t saved;

    union {
        uint64_t s[ SHA3_KECCAK_SPONGE_WORDS ];
        uint64_t sb[ SHA3_KECCAK_SPONGE_WORDS ];
    };
    unsigned byteIndex;
    unsigned wordIndex;
    unsigned capacityWords; // r = 1600 - 2 * 256; capacity = 256 * 2
} sha3_context;

/* For Init or Reset call these: */
void sha3_Init256 ( void *priv );

void sha3_Update ( void *priv, void const *bufIn, size_t len );

void const *sha3_Finalize ( void *priv );

#endif
