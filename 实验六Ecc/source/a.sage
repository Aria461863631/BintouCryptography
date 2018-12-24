K.<a> = GF( 2^8 )
E = EllipticCurve( K, [1,(1),0,0,(a^2+1)])
print E.abelian_group(debug=True)
