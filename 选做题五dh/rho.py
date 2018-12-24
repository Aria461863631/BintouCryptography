# pollard's rho algorithm for discrete logarithms


def inverse(x, m):
    a, b, u = 0, m, 1
    while x > 0:
        x, a, b, u = b % x, u, x, a - b // x * u
    if b == 1:
        return a % m
    return 0  # must be coprime


def dlog(g, t, p):
    # l such that g**l == t (mod p), with p prime
    # algorithm due to Crandall/Pomerance "Prime Numbers" sec 5.2.2
    from fractions import gcd

    def f(xab):
        x, a, b = xab[0], xab[1], xab[2]
        if x < p/3:
            return [(t*x) % p, (a+1) % (p-1), b]
        if 2*p/3 < x:
            return [(g*x) % p, a, (b+1) % (p-1)]
        return [(x*x) % p, (2*a) % (p-1), (2*b) % (p-1)]
    i, j, k = 1, [1, 0, 0], f([1, 0, 0])
    while j[0] <> k[0]:
        print i, j, k
        i, j, k = i+1, f(j), f(f(k))
    print i, j, k
    d = gcd(j[1] - k[1], p - 1)
    if d == 1:
        return ((k[2]-j[2]) % (p-1) * inverse((j[1]-k[1]) % (p-1), p-1)) % (p-1)
    m, l = 0, ((k[2]-j[2]) % ((p-1)/d) * inverse((j[1]-k[1]) %
                                                 ((p-1)/d), (p-1)/d)) % ((p-1)/d)
    while m <= d:
        print m, l
        if pow(g, l, p) == t:
            return l
        m, l = m+1, (l+((p-1)/d)) % (p-1)
    return False


p = 93450983094850938450983409623

q = (p - 1) // 2

g = 93450983094850938450983409621

Alice = 45416776270485369791375944998

Bob = 15048074151770884271824225393

gq = pow(g, q, p)
print gq
gx1 = pow(Alice, q, p)
print gx1

print dlog(gq, gx1, p)
