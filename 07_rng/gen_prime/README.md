# generate prime

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  . setup rng ... ok

  ! Generating large primes may take minutes!

  . Verifying that Q = (P-1)/2 is prime ... ok

  . generate 512 bit prime data ... ok
     DE 41 1D 39 E1 72 2B 80 3E C4 3C A7 98 1E 54 7F
     18 EC 19 D7 1D 8B 84 2B E7 62 E4 02 10 35 30 B6
     84 25 41 62 69 F0 98 E9 D7 64 36 2D 7A 59 D5 F9
     B1 48 84 99 EB 0D 4F 55 40 86 F5 27 2D 18 4E F7
```