#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "KeccakHash.h"

#define BUF_SIZE 4096

int main ( int argc, char **argv ) {
    clock_t begin = clock ();

    unsigned char *p, output[ 32 ];
    int n, i;
    struct stat st;

    Keccak_HashInstance ins;
    Keccak_HashInitialize_SHA3_256 ( &ins );

    // while ((n = fread(input, sizeof(unsigned char), BUF_SIZE, infile)) && n > 0)

    int fd = open ( argv[ 1 ], O_RDONLY );
    i = fstat ( fd, &st );
    p = (unsigned char *)( mmap ( NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0 ) );
    Keccak_HashUpdate ( &ins, p, 8 * st.st_size );

    Keccak_HashFinal ( &ins, output );
    for ( i = 0; i < 32; ++i )
        printf ( "%x", output[ i ] );
    printf ( "\n" );

    munmap ( p, st.st_size );

    clock_t end = clock ();
    double time_spent = (double)( end - begin ) / CLOCKS_PER_SEC;
    printf ( "Total time = %f seconds\n", time_spent );

    return 0;
}
