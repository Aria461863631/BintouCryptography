## 选做题5：攻击Diffie-Hellman

> P =93450983094850938450983409623(模数) 
>
> G =93450983094850938450983409621（生成元） 
>
> Alice send: 45416776270485369791375944998 
>
> Bob Send: 15048074151770884271824225393 
>
> What is the shared key? 

p = 2q+1

q = 46725491547425469225491704811

**对因子2**

- $ y^q \pmod{p} \equiv (g^{x_1})^q = 93450983094850938450983409622$
- $ x_1 = 1 $

**对因子q**

- CADO-NFS https://lists.gforge.inria.fr/pipermail/cado-nfs-discuss/2018-November/000942.html
- 计算$ a^x \equiv target \pmod{p} $, 并将 $ x $ 对 $ell$ 取模
- base a 为程序自己选的, 要想计算特定g的幂, 取log(target)/log(g)
- ell需要是p-1的因子且为素数, 其他小的因子的结果通过其他方法(如上)求解
- 很快 , 30bit第一次筛选总共才不到1min

```shell
./cado-nfs.py -dlp -ell 46725491547425469225491704811 target=45416776270485369791375944998 93450983094850938450983409623 
```

- ![](/home/aria/Pictures/Screenshot from 2018-11-05 12-51-35.png)

```python
redlog=ZZ(Integers(ell)(logtarget/logg))
```

- $ x_2 =18319922375531859171613379181 $  

最后CRT

- $ x = 18319922375531859171613379181 $

**Key**

- 85771409470770521212346739540