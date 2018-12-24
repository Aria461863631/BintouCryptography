//其实就把系数带入公式就行了orz



## GF(2^8)域上的, 判断椭圆曲线是否为singular的判别式

*Definition*

- An elliptic curve $ E $ is the set of solutions to a generalized Weierstrass equation

$$
E : Y^2 + a_1 XY + a_3 Y = X^3 + a_2 X^2 + a_4 X + a_6 ,
$$

- together with an extra point $ O $.

- coefficients $ a_1 , . . . , a_6 $ are required to satisfy $ \Delta \neq 0 $
- where the *discriminant* $ \Delta $ is defined

$$
\begin{align}
b_2 & = a_1^ 2 + 4a_2 \\
b_4 & = 2a_4 + a_1 a_3 \\
b_6 & = a_3^2 + 4a_6 \\
b_8 & = a_1^2 a_6 + 4a_2 a_6 − a_1 a_3 a_4 + a_2 a_3^2 − a _4^2 \\
\Delta & = −b_2^2 b_8 − 8b_4^3 − 27b_6^2 + 9b_2 b_4 b_6
\end{align}
$$

<br/>

书上$ GF(2^8) $ 的椭圆曲线公式
$$
Y^2+XY=X^3+AX^2+B
$$
可得 $ a_1 = 1, a_2 = A, a_3 = 0, a_4 = 0, a_6 = B $

带入上面的公式, 其中域内的数乘常量看作域内的加法,如 $4*a_i = 0 * a_i = 0 $,   $ 27 * a_i = 1 * a_i = a_i $
$$
\begin{align}
b_2 & = a_1^ 2 + 4a_2 = 1 + 4a_1 = 1\\
b_4 & = 2a_4 + a_1 a_3 = 0\\
b_6 & = a_3^2 + 4a_6 = 4a_6 = 0\\
b_8 & = a_1^2 a_6 + 4a_2 a_6 − a_1 a_3 a_4 + a_2 a_3^2 − a _4^2 = B\\
\Delta & = −b_2^2 b_8 − 8b_4^3 − 27b_6^2 + 9b_2 b_4 b_6 = -B
\end{align}
$$


所以最后判断曲线是否为singular的条件就是$B \neq 0 $