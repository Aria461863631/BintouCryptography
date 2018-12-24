编程完成，并接受助教的提问。

1. Write a program to play with $\mathbb{Z}_p^*$，where p is a prime. 

   (a) What is the order of $\mathbb{Z}_p^*$.  

   p-1

   (b) Find all the generators of $\mathbb{Z}_{11}^*$ and $\mathbb{Z}_{13}^*$

    $\mathbb{Z}_{11}^*$ : 2, 6, 7, 8

    $\mathbb{Z}_{13}^*$ :  2, 6, 7, 11

   ```c++
   //Z*11									//Z*13
    1  1  1  1  1  1  1  1  1  1            1  1  1  1  1  1  1  1  1  1  1  1 
    2  4  8  5 10  9  7  3  6  1            2  4  8  3  6 12 11  9  5 10  7  1 
    3  9  5  4  1  3  9  5  4  1            3  9  1  3  9  1  3  9  1  3  9  1 
    4  5  9  3  1  4  5  9  3  1            4  3 12  9 10  1  4  3 12  9 10  1 
    5  3  4  9  1  5  3  4  9  1            5 12  8  1  5 12  8  1  5 12  8  1 
    6  3  7  9 10  5  8  4  2  1            6 10  8  9  2 12  7  3  5  4 11  1 
    7  5  2  3 10  4  6  9  8  1            7 10  5  9 11 12  6  3  8  4  2  1 
    8  9  6  4 10  3  2  5  7  1            8 12  5  1  8 12  5  1  8 12  5  1 
    9  4  3  5  1  9  4  3  5  1            9  3  1  9  3  1  9  3  1  9  3  1 
   10  1 10  1 10  1 10  1 10  1           10  9 12  3  4  1 10  9 12  3  4  1 
                                           11  4  5  3  7 12  2  9  8 10  6  1 
                                           12  1 12  1 12  1 12  1 12  1 12  1 
   ```

​     (c) How many generators do $\mathbb{Z}_{11}^*$ and $\mathbb{Z}_{13}^*$ have. 

​	 $\mathbb{Z}_{11}^*$ : 4个 , $\mathbb{Z}_{13}^*$ :  4个

​    (d) Have more computation. Then find a pattern, and make a conjecture about the number of generator of $\mathbb{Z}_{p}^*$, for any prime $p$. Prove it. 

**Theorem 28.2 (Primitive Root Theorem). **

- There are exactly $ \phi (p - 1) $ primitive roots modulo p where p is prime
- When $ Z^∗_n $ is cyclic (has a generator precisely when $ n=2,4,p^k,2p^k $ for odd primes $ p $), $ Z^∗_n $ contains $ ϕ(ϕ(n)) $ generators.

Proof

- If $ n \ \big | \ p-1 $ than $ x^n = 1 \pmod{p} $ has **exactly** n solutions with $ 0 \leq  x < p $

  - ![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545663709/Screenshot_from_2018-12-24_23-01-15.png)
  - 要想使上式成立,则 $ x^n - 1 = 0 \pmod{p} $ 一定要有n个解,即证

- 因此, 对方程 $ g^n = 1 \pmod{p} $, 如果有一个解$ g $, 则 $ g, g^2, g^3 ... g^n $都是解且是全部解

- 但其中当且仅当n次方为1的数, 即 $ ord(g^i) = n$ 的解当且仅当 $ gcd(i,n)  = 1$

  所以对大小为n的子群, 生成元有 $ \phi(n) $个  

- 因此 $ Z^{ * }_p $ 大小为 p-1, 有 $\phi(p-1) $个生成元



Code

```python
# Euler's Totient Function
def phi(n):
    result = n   # Initialize result as n

    # phi(n) = n*(1-1/p1)(1-p2)...
    p = 2
    while(p <= n):
        if (n % p == 0):
            while (n % p == 0):
                n = n // p
            result = result - result // p
        p = p + 1

    if (n > 1):
        result = result - result // n

    return (int)(result)


def count_primitive_roots_prime(n):
    return phi(phi(n))
```





2. Write a program to play with $\mathbb{Z}_n^*$. 

   (a) Given an integer $n$, construct the multiplicative group $\mathbb{Z}_n^*$; 

   (b) Find a subgroup of the group $\mathbb{Z}_n^*$;

   (c) Find a relation between the size of subgroup and the size of $\mathbb{Z}_n^*​$: $|H| \ \big | \ |G| ​$

```python
import numpy as np

#生成乘法群mod n,筛选n的因子,并剔除掉这些因子的倍数
def groupn(n):
    A = np.arange(1, n)
    p, tmp = 2, n
    while p <= n:
        if n % p == 0:
            while n % p == 0:
                n = n // p
            i = p
            while i <= tmp:
                indx = np.argwhere(A == i)
                if len(indx) != 0:
                    A = np.delete(A, indx)
                i = i + p
        p = p + 1

    if n > 1:
        indx = np.argwhere(A == n)
        A = np.delete(A, indx)

    return A


def get_order(A, n):
    l = []
    for i in range(len(A)):
        a, c = A[i], 1
        while a != 1:
            a = (a * A[i]) % n
            c = c + 1
        l.append(c)
    return l


def one_sub_group(n):
    A = groupn(n)
    order = get_order(A, n)
    print(order)

    i = 0
    while order[i] == 1 or order[i] == len(A):
        i = i+1

    x = A[i]
    l = []
    if i >= n:
        return l
    while x != 1:
        l.append(x)
        x = (x * A[i]) % n
    l.append(1)
    return l


def relation(n):
    A = groupn(n)
    S = one_sub_group(n)
    len1, len2 = len(A), len(S)
    if len1 % len2 == 0:
        print("Divide")
    else:
        print("Can't divide")
```



3. Suppose that $q$ is a prime and $p = 2*q + 1$ is also a prime. Let $g = h^{(p-1)/q} $ is not equal to $1$, where $h$ is a random number chosen from $\mathbb{Z}_p$. Certainly, $\langle g \rangle$ is a cyclic group. 

   (a) Write a python(or Sage) program to generate the cyclic group $\langle g \rangle$. 

   ```python
   import numpy as np
   import random
   import sympy
   
   
   def gengroup(q):
       p = 2*q+1
       if not sympy.isprime(p):
           return
       h, g = 0, 1
       while g == 1:
           h = random.randint(1, p-1)
           g = (h ** ((p-1) // q)) % p
       print('p=', p, 'g=', g, 'h=', h)
   
       a, cnt = g, 1
       while a != 1:
           print(a, end=' ')
           a = a * g % p
           ++cnt
       print(1)
   ```

   (b) What is the order of $\langle g \rangle$, and why?

   -  $ ord(\langle g \rangle) = q$ , because $ g = h^{(p-1)/q} = h^2 $ and $ g^q = h ^{p-1} = 1 \pmod{p} $
   -  $ ord(\langle g \rangle) \neq 2$ , although $p-1$ has factors 2, however there's only one possibility { $g^q$, 1}, but q is prime and q is even, then $ h^2 \neq g^q$
   -  if $ p = kq+1 $ still won't have $ ord(\langle g \rangle) \neq k$
   -  因为q是素数,所以q阶的群每个都是生成元且没有子群,所以不管是2还是k都不会成立

   (c) How many generators are there in the group $\langle g \rangle$? Why?

   - q-1


