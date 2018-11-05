# aes

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  cipher info setup, name: AES-128-CBC, block size: 16

  cipher aes encrypt:
        4D DF 90 12 D7 B3 89 87 45 A1 ED 98 60 EB 0F A2
        FD 2B BD 80 D2 71 90 D7 2A 2F 24 0C 8F 37 2A 27
        63 74 62 96 DD C2 BF CE 7C 25 2B 6C D7 DD 4B A8
        57 7E 09 6D BD 80 24 C8 B4 C5 A1 16 0C A2 D3 F9

  cipher info setup, name: AES-128-CTR, block size: 16

  cipher aes encrypt:
        C4 1A 1D B1 56 C0 9B 59 E8 25 D9 5B 72 FD 97 BE
        F7 06 BA C1 B8 4F F5 4E 72 88 2D 17 0B DB 53 0A
        9B 0A FD 86 41 65 73 06 6B C1 F0 52 18 FC 1D 57
        9D F4 81 F7 08 CB
```
