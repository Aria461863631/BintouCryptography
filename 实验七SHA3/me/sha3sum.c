
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "sha3.h"

static void help ( const char *argv0 ) { printf ( "To call: %s 256|384|512 file_path.\n", argv0 ); }

static void byte_to_hex ( uint8_t b, char s[ 23 ] ) {
    unsigned i = 1;
    s[ 0 ] = s[ 1 ] = '0';
    s[ 2 ] = '\0';
    while ( b ) {
        unsigned t = b & 0x0f;
        if ( t < 10 ) {
            s[ i ] = '0' + t;
        } else {
            s[ i ] = 'a' + t - 10;
        }
        i--;
        b >>= 4;
    }
}

int main ( int argc, char *argv[] ) {
    sha3_context c;
    const uint8_t *hash;
    int image_size;
    const char *file_path;
    int fd;
    struct stat st;
    void *p;
    unsigned i;

    image_size = 256;

    file_path = argv[ 1 ];
    if ( access ( file_path, R_OK ) != 0 ) {
        printf ( "Cannot read file '%s'", file_path );
        return 2;
    }

    fd = open ( file_path, O_RDONLY );
    if ( fd == -1 ) {
        printf ( "Cannot open file '%s' for reading", file_path );
        return 2;
    }
    i = fstat ( fd, &st );
    if ( i ) {
        close ( fd );
        printf ( "Cannot determine the size of file '%s'", file_path );
        return 2;
    }

    p = mmap ( NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0 );
    close ( fd );
    if ( p == NULL ) {
        printf ( "Cannot memory-map file '%s'", file_path );
        return 2;
    }

    sha3_Init256 ( &c );
    sha3_Update ( &c, p, st.st_size );
    hash = sha3_Finalize ( &c );

    munmap ( p, st.st_size );

    for ( i = 0; i < image_size / 8; i++ ) {
        char s[ 3 ];
        byte_to_hex ( hash[ i ], s );
        printf ( "%s", s );
    }
    printf ( "  %s\n", file_path );

    return 0;
}
