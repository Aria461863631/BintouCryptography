注意事项

- a和b取任意GF(2^8)内的数，即0～254, 但特别的，在这里b不能取0（一个结论）
- 需要自己定义一个无穷大的点$O$, 随便取一个值(0,0)就行
- 什么时候点不断相加会回到$O$?(得到阶)
  - 看上面的公式,保证所有的式子不会除0
  - 如果P!=Q, 则是$\lambda $ 分母 = 0时, 即$x_Q + x_P  = 0$时
  - 如果P == Q, 则是$x_P = 0$时
  - 这个时候特判一下,返回定义好的$O$就行了
- 所有的x,y,a,b都一起遵循GF(2^8)内的运算,反正上面公式的加法除法啥的都是GF(2^8)的加减乘除就是了

怎么求所有点(群阶)

- 枚举x,再枚举y,看看能不能符合椭圆曲线的公式,公式里的加乘也是GF(2^8)的哦
- 结论Hasse bound:225 - 289,不管a,b取什么阶一定在这个范围里

怎么求阶

- 不断加自己看看能不能回到$O$

怎么求生成元

- 每个点都求一遍阶
- 根据a,b不同有很多曲线没有生成元的

---

怎么用

- 所有二进制数字都会和sage一样转换为多项式的形式

加减&数乘法

```c
int a = 0, b = 1;
EllipticCurve256 E = EllipticCurve256 ( a, b );

/*
 * point plus
 */
int x1, y1, x2, y2;
x1 = 2, y1 = 48, x2 = 25, y2 = 3;
EC256 P = EC256 ( x1, y1, a, b );
EC256 Q = EC256 ( x2, y2, a, b );
EC256 sol = P + Q;
sol = P-Q;
sol = 3*Q;
cout << sol << endl;
```

没写header的zz.jpg

- pointorder_bsgs: 求点阶
- cardinality_bsgs: 求群阶
- abelian_group: 求生成元

```c
int a = 0, b = 1;
EllipticCurve256 E = EllipticCurve256 ( a, b );

E.pointorder_bsgs();
```





