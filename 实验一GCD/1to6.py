#!/usr/bin/python

import math
import numpy as np


def gcd_iter(a, b):
    if a < b:
        a, b = b, a
    while b:
        tmp = a % b
        a = b
        b = tmp
    return a


# solve ax + by = gcd(a,b)
def exgcd_matrix(a, b):
    '''
     r0, s0
     r1, s1
    '''
    # init: a + 0*b = a; 0*a + b = b;
    r0, r1, s0, s1 = 1, 0, 0, 1
    # row1 - row2 * ( a/b * b )
    # therefore a - ( a/b * b ) = a % b
    # swap row1 and row2 as gcd
    while b:
        q, a, b = a//b, b, a % b
        r0, r1 = r1, r0 - q * r1
        s0, s1 = s1, s0 - q * s1
    return a, r0, s0


def exgcd_recr(a, b):
    if b == 0:
        return a, 1, 0
    else:
        d0, x0, y0 = exgcd_recr(b, a % b)
        d, x, y = d0, y0, x0 - a//b * y0
        return d, x, y


def quick_pow(a, n):
    ans = 1
    while n:
        if n & 1:
            ans *= a
        a *= a
        n >>= 1
    return ans


a, b, n = 4, 8, 24
# print(exgcd(a, b))


# solve ax = b (mod n)
# first solve ax + ny = gcd(a,n)
# if d = gcd(a,n) | b, solvable with d solutions
def modular_linear_equation_solver(a, b, n):
    g, x, y = exgcd_matrix(a, n)
    if b % g == 0:
        x0 = x * b//g % n
        for i in range(g):
            print((x0 + i * n//g) % n, end=' ')
    else:
        print("unsolvable")


# solve ax = 1 (mod n)
# gcd(a,n)==1, solve ax + ny = 1
def inverse(a, n):
    g, y, x = exgcd_recr(n, a)
    if g == 1:
        # exgcd
        print(x % n)
        # fermat little theorem
        #print(quick_pow(a, n-2) % n)
    else:
        print("unsolvable")


print(inverse(31, 3480))


def product(X):
    if len(X) == 0:
        return 1
    while len(X) > 1:
       X = [np.prod(X[i*2:(i+1)*2]) for i in range((len(X)+1)//2)]
    return X[0]


def producttree(X):
       result = [X]
       while len(X) > 1:
         X = [np.prod(X[i*2:(i+1)*2]) for i in range((len(X)+1)//2)]
         result.append(X)
       return result


def remaindersusingproducttree(n, T):
    result = [n]
    for t in reversed(T):
        result = [result[math.floor(i/2)] % t[i] for i in range(len(t))]
    return result


def remainders(n, X):
    return remaindersusingproducttree(n, producttree(X))


def batchgcd_simple(X):
    R = remainders(product(X), [n**2 for n in X])
    return [math.gcd(r//n, n) for r, n in zip(R, X)]


def all_pairs_gcd(X):
    D = batchgcd_simple(X)
    l = []
    for i in range(len(X)):
        l.append([math.gcd(D[i], X[j]) for j in range(len(X))])
    return l
