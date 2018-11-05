# hash

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  md info setup, name: SHA256, digest size: 32

  md sha-256 digest:
        BA 78 16 BF 8F 01 CF EA 41 41 40 DE 5D AE 22 23
        B0 03 61 A3 96 17 7A 9C B4 10 FF 61 F2 00 15 AD
```