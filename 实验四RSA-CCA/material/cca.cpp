#include "dec.h"
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
using namespace std;

/*bn : BigNumber*/
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

/**
 * 快速幂取模,输入a, b, c
 * 返回: a^b mod c
 */
bn F_exp ( bn a, bn b, bn c ) {
    bn ans = 1;
    a %= c;
    while ( b != 0 ) {
        if ( b % 2 != 0 )
            ans = ( ans * a ) % c;
        a = ( a * a ) % c;
        b >>= 1;
    }
    return ans;
}
/**
 * 快速乘取模,输入a, b, c
 * 返回: a*b mod c
 */
bn F_mul ( bn a, bn b, bn c ) {
    bn ans = 0;
    a %= c;
    while ( b != 0 ) {
        if ( b % 2 != 0 )
            ans = ( ans + a ) % c;
        a = ( a + a ) % c;
        b >>= 1;
    }
    return ans;
}

int main () {
    bn N ( N_str, 10 );         // 模数N
    bn e ( e_str, 10 );         // 公钥组成成分 e
    bn c_star ( c_star_s, 10 ); // 密文c*
    bn m_text ( m_text_s, 10 ); // 待解m*

    bn cpie = F_exp ( 2, e, N ); // c"  即2^e
    std::cout << cpie.get_str () << std::endl;
    /*bn feasible_cipher = ((c_star % N) * (cpie % N)) % N*/
    bn feasible_cipher = F_mul ( c_star, cpie, N );
    std::cout << feasible_cipher.get_str () << std::endl;

    char *m_times2 = dec ( feasible_cipher.get_str ().data () );
    bn m2 ( m_times2, 10 );
    std::cout << m2.get_str () << std::endl;
    bn res = m2 / 2;

    /**
     * n是两个大素数pq之积，且两者都不取2，故n必为奇数(因子只有1, n, p, q)
     * 因m < n, 故2m < 2n, 2m为偶数,假设2m>n,则2m%n=2m-n, 偶数-奇数=奇数
     * 而此题结果2m = 202为偶数,且不为0，所以2m<n
     */
    cout << "---- RESULT ----\n";
    cout << ( m2 % 2 != 0 ? "m wrong" : res.get_str () ) << "\n";
    return 0;
}
