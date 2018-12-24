## 选做题3：攻击RSA

**在不分解n的前提下，求d。 **

**给定： **

> e = 14058695417015334071588010346586749790539913287499707802938898719199384604316115908373997739604466972535533733290829894940306314501336291780396644520926473

> n = 33608051123287760315508423639768587307044110783252538766412788814888567164438282747809126528707329215122915093543085008547092423658991866313471837522758159 

d =

65537



**说明过程**

**Wiener's attack**

- Let $ N = pq $ with $ q < p < 2q $ , let $ d < \frac{1}{3}N^{\frac{1}{4}} $

  Given $ < N,e > $ with  ed = 1 \pmod{\varphi(N)} , the attacker can efficiently recover $ d $ using continued fractions

Proof

- **Lemma** 

  If $|\frac{a}{b}−x|< \frac{1}{2b^2} $ then $ \frac{a}{b} $ is a continued fraction approximant for $ x $.

- because $ ed \equiv 1 \pmod{\varphi(N)} $, exists k that 
  $$
  ed = 1 + k(p-1)(q-1) \iff kN-ed=k(p+q-1)-1
  $$

- divide $ dn $ to obtain
  $$
  \frac{k}{d}-\frac{e}{N} = \frac{k}{d}\Bigl(\frac1{q}+\frac1{p}-\frac1{n}\Bigr) - \frac1{dN}
  $$

- assuming $ p∼q∼\sqrt{N} $ and $ e∼N $,  If we choose $ d $ of order up to $ N^{1/4} $, specifically $ \frac13N^{1/4} $ , the condition of the LEMMA is satisfied
  $$
  \bigg|\frac{k}{d} -\frac{e}{N} \bigg| < \frac{1}{2d^2}
  $$
  then $ \frac{k}{d} $ appears among the **convergents** of $ \frac{e}{N} ​$

- Proof
  $$
  \Bigl|\frac{k}{d}-\frac{e}{n}\Bigr| \le \frac{k+1}{dn}+\frac{k}{d}\Bigl(\frac1{p}+\frac1{q}\Bigr) \sim \frac1{n} + \frac1{\sqrt{n}} \sim \frac1{\sqrt{n}}
  $$



Process

- n // e = 2 ... r1

  r1 =

  5490660289257092172332402946595087725964284208253123160534991376489797955806050931061131049498395270051847626961425218666479794656319282752678548480905213

- e // r1 = 2 ... r2

  r2 = 

  3077374838501149726923204453396574338611344870993461481868915966219788692704014046251735640607676432431838479367979457607346725188697726275039547559116047

- r1 // r2  = 1... r3

  r3 = 

  2413285450755942445409198493198513387352939337259661678666075410270009263102036884809395408890718837620009147593445761059133069467621556477639000921789166 ...

- **Convergent**  $ = \frac{k}{d} =  [ \frac{1}{2} , \frac{2}{5}, \frac{3}{7} ... \frac{27415}{65537}... ] $

- for each convergent 

  - Check if d is valid by $ ( ed - 1 ) \% k == 0 $
  - Compute  $ \varphi(N) = \frac{ed-1}{k} $ 
  - check if the equation has integer roots and find out the factorization of $ N  = pq$

  $$
  x^2 - ( (N-\varphi(N))+1)x + N = 0 \\	
  $$




https://err0rzz.github.io/2017/11/14/CTF%E4%B8%ADRSA%E5%A5%97%E8%B7%AF/

https://crypto.stackexchange.com/questions/56204/rsa-attack-with-continued-fractions-wieners-attack