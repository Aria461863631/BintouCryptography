## 实验作业7-Hash

> 实现基于SHA3的Hash程序。功能：给定文件，产生该文件的Hash值，并自动放到系统剪贴板。请给出程序的执行效率（时间），效率高、执行快者分数高。



自己写的1G30多秒。。。别人写的6秒（自闭）

### 5 steps for keccack

```c++
// L[x,y] = s[x+5y]
```

**Theta**

- ![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662888/Screenshot_from_2018-12-21_13-11-32.png)

- code

  ```c
  //bc = xor of the whole column
  for ( int i = 0; i < 5; ++i )
      bc[ i ] = s[ i ] ^ s[ i + 5 ] ^ s[ i + 10 ] ^ s[ i + 15 ] ^ s[ i + 20 ];
  
  /* Theta */
  for ( int i = 0; i < 5; ++i ) {
      //(i+4)%5 stands for previous column; (i+1)%5: col nex
      uint64_t t = bc[ ( i + 4 ) % 5 ] ^ SHA3_ROTL64 ( bc[ ( i + 1 ) % 5 ], 1 );
      for ( int j = 0; j < 25; j += 5 ) //whole col xor t, 5 times' cycle
          s[ j + i ] ^= t;
  }
  ```

**Rho** and **Phi**

- Rho: Each location bit shift with amount `g(t) mod 64`
- Phi: `L[x,y] = L[y',2x'+3y']` 

- ![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662888/Screenshot_from_2018-12-21_14-49-50.png)

- code

  ```c++
  //stands for the bits shift amount of each t
  static const unsigned keccakf_rotc[ 24 ] = 
  	{1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
  	27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44};
  
  // stands for location (x,y) above when t = 1,2,...23,24
  static const unsigned keccakf_piln[ 24 ] = 
  	{10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
  	15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1};
  
  /* Rho & Pi */
  uint64_t cor = s[ 1 ]; // start with location(1,0) where t = 1
  for ( int t = 0; t < 24; ++t ) {  // loop with increasing t
      int nex = keccakf_piln[ t ]; /* Pi: (x,y) <= (y',2x'+3y')
      							  * represent location of current t plus 1 */	
      uint64_t tmp = s[ nex ];
      s[ nex ] = SHA3_ROTL64 ( cor, keccakf_rotc[ t ] ); // Rho
      cor = tmp;
  }
  ```

**Chi**

- ![](https://res.cloudinary.com/dsmx9qa2z/image/upload/v1545662887/Screenshot_from_2018-12-21_15-15-35.png)

- code

  ```c++
  /* Chi */
  for ( int j = 0; j < 25; j += 5 ) { // j = {0, 5, 10, 15, 20}
      for ( int i = 0; i < 5; ++i )
          bc[ i ] = s[ j + i ];     // whole row
      for ( int i = 0; i < 5; ++i ) // nex1 and nex2 row
          s[ j + i ] ^= ( ~bc[ ( i + 1 ) % 5 ] ) & bc[ ( i + 2 ) % 5 ];
  }
  ```

**Iota**

- code

  ```c
  /* Iota */
  s[ 0 ] ^= keccakf_rndc[ round ];
  ```



---



