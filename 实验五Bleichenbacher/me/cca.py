#https://github.com/diogt52/bleichenbacher_attack


def egcd(a, b):
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


def ceil(a, b): return -(-a//b)


def floor(a, b): return a//b


def inverse(x, n): return egcd(x, n)[1]


def cca(oracle):
    k, n, cipher = oracle.k, oracle.n, oracle.cipher

    #def attackInfo(m_new, s_new): print("Info:", "set_length:" str(len(m_new)), "cur_s", str(s_new))

    def oracle_request(c, s):
        return oracle.query(c * oracle.encryption(s))

    '''
    改掉PKCS,取最高位的范围
    '''

    B = pow(2, (k-2))
    B2 = B
    B3 = 2 * B

    '''
    Step 1: Blinding.
    c0 ← c(s0)
    e mod n
    M0 ← {[2B, 3B − 1]}
    i ← 1.
    '''
    c0 = cipher
    m0 = {(B2, B3-1)}
    i = 1

    #original cipher: c0
    s_0 = 1
    #while not oracle_request(c0, s_0):
    #    s_0 += 1

    s_old = 1
    step = 1
    while True:
        '''
        Step 2.a:
        first step, choose s1 ≥ n/(3B)
        '''
        if i == 1:
            s_tmp = ceil(n, B3)
            while not oracle_request(c0, s_tmp):
                s_tmp += 1

        elif i > 1 and len(m0) >= 2:
            s_tmp = s_old + step
            #跨度,否则搜索很慢
            step *= 2
            while not oracle_request(c0, s_tmp):
                s_tmp += 1

        elif len(m0) == 1:
            a, b = next(iter(m0))
            cond = False

            r = ceil(2 * (b * s_old - B2), n)
            while not cond:
                s_lb = ceil(B2 + r*n, b)
                #+1否则死循环
                s_ub = floor(B3 + r * n, a) + 1
                for s in range(s_lb, s_ub):
                    if oracle_request(c0, s):
                        cond = True
                        s_tmp = s
                        break
                r += 1

        '''
        Step 3: Narrowing the set of solutions.
        '''
        m_new = set()

        for a, b in m0:
            min_r = ceil(a * s_tmp - B3 + 1, n)
            max_r = floor(b*s_tmp - B2, n)
            for r in range(min_r, max_r+1):
                la = max(a, ceil(B2+r*n, s_tmp))
                ub = min(b, floor(B3-1+r*n, s_tmp))
                if la <= ub:
                    m_new |= {(la, ub)}

        '''
        Step 4: Computing the solution.
        '''
        #attackInfo(m_new, s_tmp)
        if len(m_new) == 1:
            a, b = next(iter(m_new))

            if a == b:
                print("s_old,n: ", s_old, n)
                print("inverse: ", inverse(s_old, n))
                return a*inverse(s_0, n)

        i += 1
        s_old = s_tmp
        m0 = m_new
