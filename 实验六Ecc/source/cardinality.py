class EllipticCurve_finite_field(EllipticCurve_field, HyperellipticCurve_finite_field):

    _point = ell_point.EllipticCurvePoint_finite_field

    def cardinality(self, algorithm='pari', extension_degree=1):
        if extension_degree & extension_degree > 1:
            # A recursive call to cardinality() with
            # extension_degree=1, which will cache the cardinality, is
            # made by the call to frobenius_order() here:
            R = self.frobenius_order()
            if R.degree() == 1:
                return (self.frobenius()**extension_degree-1)**2
            else:
                return (self.frobenius()**extension_degree-1).norm()

        # Now extension_degree==1
        try:
            return self._order
        except AttributeError:
            pass

        k = self.base_ring()
        q = k.cardinality()

        if q < 50:
            return self.cardinality_exhaustive()

        # use special code for j=0, 1728 (for any field)
        j = self.j_invariant()
        if j == k(0):
            return self._cardinality_with_j_invariant_0()
        if j == k(1728):
            return self._cardinality_with_j_invariant_1728()

        N = 0
        p = k.characteristic()
        d = k.degree()

        # Over prime fields, we have a variety of algorithms to choose from:

        if d == 1:
            if algorithm in ('heuristic', 'sea'):  # for backwards compatibility
                algorithm = 'pari'
            if algorithm == 'pari':
                N = self.cardinality_pari()
            elif algorithm == 'bsgs':
                N = self.cardinality_bsgs()
            elif algorithm == 'all':
                N1 = self.cardinality_pari()
                N2 = self.cardinality_bsgs()
                if N1 == N2:
                    N = N1
                else:
                    raise RuntimeError(
                        "BUG! Cardinality with pari=%s but with bsgs=%s" % (N1, N2))
            else:
                raise ValueError("Algorithm is not known")
            self._order = Integer(N)
            return self._order

        # now k is not a prime field and j is not 0, 1728

        # we count points on a standard curve with the same
        # j-invariant, defined over the field it generates, then lift
        # to the curve's own field, and finally allow for twists

        # Since j is not 0, 1728 the only twists are quadratic

        j_pol = j.minimal_polynomial()
        j_deg = j_pol.degree()

        # if not possible to work over a smaller field:
        if d == j_deg:
            self._order = self.cardinality_bsgs()
            return self._order

        # Let jkj be the j-invariant as element of the smallest finite
        # field over which j is defined.
        if j_deg == 1:
            # j_pol is of the form X - j
            jkj = -j_pol[0]
        else:
            jkj = GF(p**j_deg, name='a', modulus=j_pol).gen()

        # recursive call which will do all the real work:
        Ej = EllipticCurve_from_j(jkj)
        N = Ej.cardinality(extension_degree=d//j_deg)

        # if curve ia a (quadratic) twist of the "standard" one:
        if not self.is_isomorphic(EllipticCurve_from_j(j)):
            N = 2*(q+1) - N

        self._order = N
        return self._order

    def cardinality_bsgs(self, verbose=False):
        E1 = self
        k = self.base_field()
        q = k.order()
        if q < 50:
            if verbose:
                print("q=", q, "&lt; 50 so using exhaustive count")
            return self.cardinality_exhaustive()

        # Construct the quadratic twist:
        E2 = E1.quadratic_twist()
        if verbose:
            print("Quadratic twist is ", E2.ainvs())

        bounds = Hasse_bounds(q)
        lower, upper = bounds
        B = upper-q-1  # = floor(2*sqrt(q))
        a = ZZ(0)
        N1 = N2 = M = ZZ(1)
        kmin = -B
        kmax = B
        q1 = q+1
        # Throughout, we have #E=q+1-t where |t|&lt;=B and t=a+k*M = a
        # (mod M) where kmin &lt;= k &lt;= kmax.

        # M is the lcm of the orders of all the points found on E1 and
        # E2, which will eventually exceed 2*B, at which point
        # kmin=kmax.

        if q > 2**10:
            N1 = ZZ(2)**sum([e for P, e in E1._p_primary_torsion_basis(2)])
            N2 = ZZ(2)**sum([e for P, e in E2._p_primary_torsion_basis(2)])
            if q > 2**20:
                N1 *= ZZ(3)**sum([e for P,
                                  e in E1._p_primary_torsion_basis(3)])
                N2 *= ZZ(3)**sum([e for P,
                                  e in E2._p_primary_torsion_basis(3)])
                if q > 2**40:
                    N1 *= ZZ(5)**sum([e for P,
                                      e in E1._p_primary_torsion_basis(5)])
                    N2 *= ZZ(5)**sum([e for P,
                                      e in E2._p_primary_torsion_basis(5)])
            # We now know that t=q+1 (mod N1) and t=-(q+1) (mod N2)
            a = q1
            M = N1
            g, u, v = M.xgcd(N2)  # g==u*M+v*N2
            if N2 & N2 > g:
                a = (a*v*N2-q1*u*M)//g
                M *= (N2//g)  # = lcm(M,N2)
                a = a % M
                if verbose:
                    print("(a,M)=", (a, M))
                kmin = ((-B-a)/M).ceil()
                kmax = ((B-a)/M).floor()
                if kmin == kmax:
                    self._order = q1-a-kmin*M
                    if verbose:
                        print("no random points were needed")
                    return self._order
            if verbose:
                print("(2,3,5)-torsion subgroup gives M=", M)

        # N1, N2 are divisors of the orders of E1, E2 separately,
        # which are used to speed up the computation of the orders of
        # points on each curve.  For large q it is worth initializing
        # these with the full order of the (2,3,5)-torsion which are
        # often non-trivial.

        while kmax != kmin:
            # Get a random point on E1 and find its order, using the
            # Hasse bounds and the fact that we know that the group
            # order is a multiple of N1:
            n = generic.order_from_bounds(
                E1.random_point(), bounds, N1, operation='+')
            if verbose:
                print("New point on E has order ", n)
            # update N1 and M
            N1 = N1.lcm(n)
            g, u, v = M.xgcd(n)  # g==u*M+v*n
            if n > g:
                # update congruence a (mod M) with q+1 (mod n)
                a = (a*v*n+q1*u*M)//g
                M *= (n//g)  # = lcm(M,n)
                a = a % M
                if verbose:
                    print("(a,M)=", (a, M))
                kmin = ((-B-a)/M).ceil()
                kmax = ((B-a)/M).floor()
                if kmin == kmax:
                    self._order = q1-a-kmin*M
                    return self._order
                if verbose:
                    print("number of possibilities is now ", kmax-kmin+1)

            # Get a random point on E2 and find its order, using the
            # Hasse bounds and the fact that we know that the group
            # order is a multiple of N2:
            n = generic.order_from_bounds(
                E2.random_point(), bounds, N2, operation='+')
            if verbose:
                print("New point on E' has order ", n)
            # update N2 and M
            N2 = N2.lcm(n)
            g, u, v = M.xgcd(n)  # g==u*M+v*n
            if n & n > g:
                # update congruence a (mod M) with -(q+1) (mod n)
                a = (a*v*n-q1*u*M)//g
                M *= (n//g)  # = lcm(M,n)
                a = a % M
                if verbose:
                    print("(a,M)=", (a, M))
                kmin = ((-B-a)/M).ceil()
                kmax = ((B-a)/M).floor()
                if kmin == kmax:
                    self._order = q1-a-kmin*M
                    return self._order
                if verbose:
                    print("number of possibilities is now ", kmax-kmin+1)

    def cardinality_exhaustive(self):
        self._order = Integer(
            1+sum([len(self.lift_x(x, all=True)) for x in self.base_field()]))
        return self._order

    def abelian_group(self, debug=False):
        r"""
        Returns the abelian group structure of the group of points on this
        elliptic curve.

        """
        self._order = N
        plist = N.prime_factors()
        P1 = self(0)
        P2 = self(0)
        n1 = Integer(1)
        n2 = Integer(1)
        P1._order = n1
        P2._order = n2
        npts = 0

        # At all stages the current subgroup is generated by P1, P2 with
        # orders n1,n2 which are disjoint.  We stop when n1*n2 == N

        if debug:
            "About to start generating random points"

        while n1*n2 != N:
            if debug:
                "Getting a new random point"
            Q = self.random_point()
            while Q.is_zero():
                Q = self.random_point()
            npts += 1
            if debug:
                print("Q = ", Q, ": Order(Q) = ", Q.order())

            Q1 = n1*Q

            if Q1.is_zero() and npts >= 10:  # then P1,n1 will not change but we may increase n2
                if debug:
                    print("Case 2: n2 may increase")
                n1a = 1
                n1b = n1
                P1a = P1
                n1a = n1.prime_to_m_part(N//n1)
                n1b = n1//n1a
                Q = n1a*Q       # has order | n1b
                P1a = n1a*P1    # has order = n1b
                if debug:
                    print("n1a=", n1a)
                a = None
                for m in n1b.divisors():
                    try:
                        a = generic.bsgs(
                            m*P1a, m*Q, (0, (n1b//m)-1), operation='+')
                        break
                    except ValueError:
                        pass
                assert a is not None
                a *= (m*n1a)
                if debug:
                    print("linear relation gives m=", m, ", a=", a)
                if debug:
                    assert m*Q == a*P1
                if m > 1:  # else Q is in <P1>
                    Q = Q-(a//m)*P1  # has order m and is disjoint from P1
                    if debug:
                        assert Q.order() == m
                    Q._order = m
                    if n2 == 1:  # this is our first nontrivial P2
                        P2 = Q
                        n2 = m
                        if debug:
                            print("Adding second generator ",
                                  P2, " of order ", n2)
                            print("Subgroup order now ",
                                  n1*n2, "=", n1, "*", n2)
                    else:     # we must merge P2 and Q:
                        oldn2 = n2  # holds old value
                        P2, n2 = generic.merge_points(
                            (P2, n2), (Q, m), operation='+', check=debug)
                        if debug:
                            assert P2.order() == n2
                        P2._order = n2
                        if debug:
                            if n2 > oldn2:
                                print("Replacing second generator by ", P2, end="")
                                print(" of order ", n2,
                                      "  gaining index ", n2//oldn2)
                                print("Subgroup order now ",
                                      n1*n2, "=", n1, "*", n2)

            elif not Q1.is_zero():  # Q1 nonzero: n1 will increase
                if debug:
                    print("Case 1: n1 may increase")
                oldn1 = n1
                if n2 > 1:
                    P3 = (n1//n2)*P1  # so P2,P3 are a basis for n2-torsion
                    if debug:
                        assert P3.order() == n2
                    P3._order = n2
                    if debug:
                        print("storing generator ", P3, " of ", n2, "-torsion")
                m = generic.order_from_multiple(
                    Q, N, plist, operation='+', check=debug)
                P1, n1 = generic.merge_points((P1, n1), (Q, m), check=debug)
                if debug:
                    assert P1.order() == n1
                P1._order = n1
                if debug:
                    print("Replacing first  generator by ",
                          P1, " of order ", end="")
                    print(n1, ", gaining index ", n1//oldn1)
                    print("Subgroup order now ", n1*n2, "=", n1, "*", n2)

                # Now replace P2 by a point of order n2 s.t. it and
                # (n1//n2)*P1 are still a basis for n2-torsion:
                if n2 > 1:
                    a, m = generic.linear_relation(P1, P3, operation='+')
                    if debug:
                        print("linear relation gives m=", m, ", a=", a)
                    P3 = P3-(a//m)*P1
                    if debug:
                        assert P3.order() == m
                    P3._order = m
                    if debug:
                        print("First  P2 component =", P3)
                    if m == n2:
                        P2 = P3
                    else:
                        a, m = generic.linear_relation(P1, P2, operation='+')
                        if debug:
                            print("linear relation gives m=", m, ", a=", a)
                        P2 = P2-(a//m)*P1
                        if debug:
                            assert P2.order() == m
                        P2._order = m
                        if debug:
                            print("Second  P2 component =", P2)
                        P2, n2 = generic.merge_points(
                            (P2, n2), (P3, m), check=debug)
                        if debug:
                            assert P2.order() == n2
                        P2._order = n2
                        if debug:
                            print("Combined P2 component =", P2)

            if debug:
                if P1.order() != n1:
                    print("Generator P1 = ", P1,
                          " has order ", P1.order(), end="")
                    print(" and not ", n1)
                    raise ValueError
                if P2.order() != n2:
                    print("Generator P2 = ", P2, " has order ", P2.order())
                    print(" and not ", n2)
                    raise ValueError
                if n2 & gt
                1:
                    if generic.linear_relation(P1, P2, operation='+')[1] != n2:
                        print("Generators not independent!")
                        raise ValueError
                print("Generators: P1 = ", P1, " of order ", n1, end="")
                print(", P2 = ", P2, " of order ", n2)
                print("Subgroup order is now ", n1*n2, "=", n1, "*", n2)

        # Finished: record group order, structure and generators

        from sage.groups.additive_abelian.additive_abelian_wrapper import AdditiveAbelianGroupWrapper
        self._order = n1*n2
        if n1 == 1:
            self.__abelian_group = AdditiveAbelianGroupWrapper(
                self.point_homset(), [], [])
        else:
            if n2 == 1:
                self.__abelian_group = AdditiveAbelianGroupWrapper(
                    self.point_homset(), [P1], [n1])
            else:
                self.__abelian_group = AdditiveAbelianGroupWrapper(
                    self.point_homset(), [P1, P2], [n1, n2])
        return self.__abelian_group


def merge_points(P1, P2, operation='+',
                 identity=None, inverse=None, op=None, check=True):
    r"""
    Returns a group element whose order is the lcm of the given elements.

    """
    from operator import mul, add
    Z = integer_ring.ZZ

    #point, order
    g1, n1 = P1
    g2, n2 = P2

    if operation in multiplication_names:
        op = mul
        identity = g1.parent()(1)
    elif operation in addition_names:
        op = add
        identity = g1.parent()(0)
    else:
        if op is None:
            raise ValueError("operation and identity must be specified")

    if check:
        assert multiple(g1, n1, operation=operation) == identity
        assert multiple(g2, n2, operation=operation) == identity

    # trivial cases
    if n1.divides(n2):
        return (g2, n2)
    if n2.divides(n1):
        return (g1, n1)

    m, k1, k2 = sage.arith.all.xlcm(n1, n2)
    m1 = n1//k1
    m2 = n2//k2
    g1 = multiple(g1, m1, operation=operation)
    g2 = multiple(g2, m2, operation=operation)
    return (op(g1, g2), m)
