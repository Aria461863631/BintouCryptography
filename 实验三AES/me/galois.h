#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int galois_single_multiply ( int a, int b, int w );
extern int galois_single_divide ( int a, int b, int w );
extern int galois_log ( int value, int w );
extern int galois_ilog ( int value, int w );

extern int galois_create_log_tables ( int w ); /* Returns 0 on success, -1 on failure */
extern int galois_logtable_multiply ( int x, int y, int w );
extern int galois_logtable_divide ( int x, int y, int w );

void unix_error ( char *msg ) /* Unix-style error */
{
    fprintf ( stderr, "%s: %s\n", msg, strerror ( errno ) );
    exit ( 0 );
}
