#include "SimpleFIPS202.h"
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
int main ( int argc, char *argv[] ) {
    clock_t begin = clock ();

    unsigned char *input, output[ 32 ];
    struct stat st;

    int fd = open ( argv[ 1 ], O_RDONLY );
    int i = fstat ( fd, &st );
    input = (unsigned char *)( mmap ( NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0 ) );

    SHA3_256 ( output, input, st.st_size );

    for ( i = 0; i < 32; ++i )
        printf ( "%x", output[ i ] );
    printf ( "\n" );

    munmap ( input, st.st_size );

    clock_t end = clock ();
    double time_spent = (double)( end - begin ) / CLOCKS_PER_SEC;
    printf ( "Total time = %f seconds\n", time_spent );
}
