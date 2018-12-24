# -*- coding: UTF-8 -*-
#定义多项式的变量是a
K.<a> = GF(2^8)

#第一个括号里是参数a,第二个是b,要写成多项式的形式
E = EllipticCurve(K,[1,(a),0,0,(a^2+1)])
print E

#点个数
print E.cardinality()

#所有点
print E.points()

P = E(a , a^6 + a^3 + a^2 )
Q = E(a^5 + a^3 + 1 , a^7 + a^4 + a^2 + 1 )

print P.order()
print P+Q




