# generate random

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  . setup rng ... ok
  
  . generate 64 byte random data ... ok
     D4 E7 5B 98 FB 45 82 A0 57 E9 EA 7A FD 17 D1 A4
     29 68 9D E4 92 BD 75 03 AF 5F 7B 24 2C E1 31 DC
     BA 82 2C 2A 4C 77 57 63 60 C4 14 27 FB DC 8B 3C
     9B EE 45 FD C4 4A A0 5C 2E 4A 0D B5 F1 79 28 D0
```