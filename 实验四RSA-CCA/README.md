## 实验课程作业4（RSA）

**Programming assignment.** You are given an oracle-access to a function dec(c) that inverts the RSA N,d function: on input c it computes $ m = c^d \ mod \ N $ **for all but one** ciphertext.  We call this ciphertext a challenge-ciphertext $ c^∗$ . The parameters $ (N, e, d, c^∗ ) $ are fixed. You’ll find all public parameters in the file `params.txt`. Your task is to decrypt the challenge $ c^∗$ 

**题意**

有dec能解密除了$c^*$以外所有的密文, 现在让你用它用它来解密$c^*$

To accomplish the task you should follow the instruction below (Important! You will need to have the GMP library installed on your machine (www.gmplib.org):



**Instructions (for Linux):**

1. Download the two files `dec.o` and `dec.h` from the web-page.

It provides the function

```c
char∗ dec ( const char ∗ c_inp )
```

that returns the decryption of a ciphertext c_inp given as a string for fixed (N, d). You can also provide a ciphertext of the GMP long int type by calling

```c
char∗ dec ( mpz t ∗ c_inp )
```

2. To use the above function, either create your own .cpp file and include `dec.h` as a header or download the template file `hw1.cpp` from the web-site. To compile this .cpp file with the `dec.o` run in terminal

```sh
g++ hw1.cpp dec.o −lgmp
```

Don’t forget to link it with the GMP library!

3. As the result, you should get `a.out` file which you can then execute.

As this is an attack on a public key cryptosystem and you are given $ e $, you should implement the corresponding encryption function by yourself. You should submit both the resulting `m = dec(c∗ )` and your code.



---



Sol

- 101

Process

- Construct $ C' = 2^eC^* $
- send $C' $ to `dec` and obtain $ M' = 2M $, so $ M = M' \times 2^{-1}$

Code

```c
#include "dec.h"
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>

typedef mpz_class bn;

const char *N_str =
    "1071508607186267320948425049060001810561404811705533607443750388370351051124936122493198378815"
    "6958581275946729175531469002933770824382865926730400902798743137187335810705309884635534159797"
    "7322595205943373851868976298683624144753090015077192592725086694196765086066308233512429642050"
    "44695669333236417591";

const char *e_str =
    "1033507197783958849532434330701272124186803034586769923345150080902155598940302810374322178241"
    "7440900848403102247012012875905268518785845678756696925714007988778268752026049276281025329038"
    "0710870214468348565666875377299183728637292920159788095066074117110737168986916602118354038008"
    "10547133032654209857";

const char *c_star_s =
    "7757895682554477140132479188344751986796539177416753369255993352652055979745568787966196883914"
    "9015340055369071515682518641008346723944186793036236875907282474251282142395916627073691413060"
    "4102452801162684877374802075310241079026986641176079329871431448404341153307957496668749957011"
    "118721172866996397";

const char *m_text_s = "2";

int main () {
    bn N ( N_str, 10 );
    bn e ( e_str, 10 );
    bn c_star ( c_star_s, 10 );
    bn m_text ( m_text_s, 10 );
    bn u_constant ( "2", 10 );

    bn c_prime; // c' = 2^e
    mpz_powm ( c_prime.get_mpz_t (), u_constant.get_mpz_t (), e.get_mpz_t (), N.get_mpz_t () );
    bn feasible_c; // fc = {c*} * c' = {2m}^e mod(n)
    mpz_mul ( feasible_c.get_mpz_t (), c_prime.get_mpz_t (), c_star.get_mpz_t () );
    mpz_mod ( feasible_c.get_mpz_t (), feasible_c.get_mpz_t (), N.get_mpz_t () );

    // m2 = 2 * m
    char *m2_text = dec ( feasible_c.get_str ().data () );
    bn m2 ( m2_text, 10 );
    bn sol;
    mpz_divexact ( sol.get_mpz_t (), m2.get_mpz_t (), u_constant.get_mpz_t () );

    std::cout << sol.get_str () << std::endl;
    return 0;
}
```



https://runzhizeng.github.io/2015/05/05/CCA%20of%20RSA/