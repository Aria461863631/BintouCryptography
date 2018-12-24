### 常规作业1-代数

1. Prove the following proposition. Let $\mathbb{G}$ be a group, for any two elements $a, b \in \mathbb{G}$. Then the equation $ax = b$ and $xa = b $ have unique solutions in $\mathbb{G}$.

Proof

- 因为$\mathbb{G}$ 是群, $a, b \in \mathbb{G}$, 所以 $ a , b $ 都有唯一逆元
- 解 $ ax = b $ , 同时左乘 $ a^{-1} $, 得到唯一解 $ a^{-1}b $
- 同理, 解 $ xa = b $ , 同时右乘 $ a^{-1} $, 得到唯一解 $ ba^{-1} $



2. Prove the following proposition. Let $\mathbb{G}$ be a group, and $a, b, c \in \mathbb{G}$. Then $ba = ca$ implies $b = c$ and $ab = ac$ implies $b = c$.

Proof

- 因为$\mathbb{G}$ 是群, $a, b, c \in \mathbb{G}$, 所以 $ a , b, c $ 都有唯一逆元
- 由 $ba = ca$ ,同时右乘 $ a^{-1} $, 得到 $ b = c $
- 由 $ab = ac$ ,同时左乘 $ a^{-1} $, 得到 $ b = c $



3. Prove the Fermat's Little Theorem using Group Theory.

Proof

- $ a^{p-1} = 1 $ where $ a \in Z_p^{ * } $
- 由Lagrange Theorem: $H = <g> $ where $ g \in \mathbb{G} $ 有 $ |G| = [G : H]|H|$
- $ g^{p-1} = g ^{|G|} = g^{[G : H]|H|} = 1^{[G : H] } = 1 $



4. Prove the Euler's Theorem using Group Theory.

- $ a^{\phi(n)} = 1 $ where $ a \in Z_n^{ * } $
- 由Lagrange Theorem: $H = <g> $ where $ g \in \mathbb{G} $ 有 $ |G| = [G : H]|H|$
- $   g^{\phi(n)}= g ^{|G|} = g^{[G : H]|H|} = 1^{[G : H] } = 1 $



### 常规作业2-AES

课本 第五章 Problems 6.2 和6.13. 请提交纸质版。

**6.2**

$ \{01\}^{-1} $ in $ GF(2^8) $ is $ \{01\} $

Verify: $ X * B = $ 0x1F, 0x1F xor {63} = 0x7C = $ S_{[1,2]} $

**6.13**

