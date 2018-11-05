# ecdsa

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

### 用例输出
```
  . setup rng ... ok

  1. hash msg ... ok
  2. ecdsa generate keypair:
     04 BF 99 30 5D 99 56 D1 0A B4 25 9F 13 57 D8 BB
     45 49 89 CE 50 6B 46 2C 25 BF 05 16 EA 91 30 5D
     06 D7 36 AD BC FE EC A9 7E 18 1F F2 48 DC BC FE
     F4 44 D2 CC 35 45 24 3C C1 C2 DB 11 F5 95 45 1C
     55 C5 83 9B A5 4B E5 6F 31 9B 52 40 BC DD ED 48
     E7 3D D1 C8 E9 86 35 4A 3E 27 2E 40 E0 8B 8E B6
     9E
  3. ecdsa generate signature:
     87 9E 59 05 6C D8 3D CB BE 74 48 4E 7E 06 EE 36
     22 0F 73 C7 E8 80 0F 76 FE 66 35 36 9E 53 A8 3D
     23 91 6A 4D EB C4 1C 6B D2 61 EA DD B9 B1 CA 9E
     E1 27 5C 38 69 90 08 14 FD 86 A5 60 F1 56 36 79
  4. ecdsa verify signature ... ok
```