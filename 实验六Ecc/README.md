## 实验课程作业6-ECC

> 编程实现以下椭圆曲线相关功能。考虑在GF(2^8)上的E(GF(2^8))，参考CANS的310-312页。

曲线方程
$$
y^2 + xy = x^3 + ax^2 + b
$$

---

> 2、现实E上的点加法；即实现求：P+P、P+Q和nP的程序

![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662936/Screenshot_from_2018-11-30_10-59-18.png)

![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662935/Screenshot_from_2018-11-30_10-59-37.png)

再就是上次A和B取什么值的时候singular

根据椭圆曲线的通式

*Definition*

- An elliptic curve  E  is the set of solutions to a generalized Weierstrass equation

$$
E : Y^2 + a_1 XY + a_3 Y = X^3 + a_2 X^2 + a_4 X + a_6 ,
$$

- together with an extra point $ O $.
- coefficients  $ a_1 , . . . , a_6 $ are required to satisfy $ \Delta \neq 0 $
- where the discriminant $ \Delta $  is defined

$$
\begin{align}
b_2 & = a_1^ 2 + 4a_2 \\
b_4 & = 2a_4 + a_1 a_3 \\
b_6 & = a_3^2 + 4a_6 \\
b_8 & = a_1^2 a_6 + 4a_2 a_6 − a_1 a_3 a_4 + a_2 a_3^2 − a _4^2 \\
\Delta & = −b_2^2 b_8 − 8b_4^3 − 27b_6^2 + 9b_2 b_4 b_6
\end{align}
$$

代入得到 $\Delta = -B $

所以B!=0就行了

----



> 1、	统计该椭圆曲线上点的个数，即点群的Order。

挑了最简单的写了下orz, 其他的....看起来好难....

**Baby Step Giant Step**

![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662935/Screenshot_from_2018-11-30_11-21-55.png)

![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662935/Screenshot_from_2018-11-30_11-22-45.png)



其中**第一步**要取一个随机的点

- 随机选择一个坐标x, 找到对应的y (lift_x函数)
- 我们已知x后, 带入曲线可以得到 $ y^2 + xy - f(x) = 0 $ where $ f(x) = x^3 + ax^2 + b $, 求该方程root
- 两个问题
  - 不能直接带入 $ \Delta = b^2 - 4ac $, 因为我们现在是chracteristic = 2的群, 加法减法相当于亦或..反正算进去就没项了
  - 方程两边同时除$x^2$ , 得到 $ \left( \frac{y}{x} \right)^2 + \left( \frac{y}{x}\right) - c = 0$
  - 然后好像正常解就是completing the square变成 $ (y - a)( y-b ) $这样之类的, 不会, 直接弄了个map把所有数的$x^2+x$算了一遍qwq,到时候直接查表......

**有了随机点之后**就可以带入上面的bsgs算法求一个点的order

多取几个随机点求他们order的lcm = n1, 当n1在Hasse bound里有唯一的倍数的时候我们的order就出来了 

---



> 3、这个曲线上的点群是循环群吗？如果是，求一个生成元。

不一定是循环群 , 看 A 和 B 的取值

如果是循环群

- 选取多个随机的点, 求他们的阶
- 如当前我们构造的生成元order(P1) = n1, 找到了一个点Q where order(Q)与order(P1)有互素的部分
- 则合并两个点, 新的order(P1) = lcm(P1.order, Q.order)
- 直到P1的order = 群大小

不是循环群

- 要补全阶的另一部分
- 找到Q不属于我们现在生成元P1所生成的子群, 有一对a 和 m 使 aP1 = mQ, 其中m>1
- 我们就找到了剩下的部分m

---



> 4、编程画出曲线上所有的点

lande..............



