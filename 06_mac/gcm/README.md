# gcm

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  cipher info setup, name: AES-128-GCM, block size: 16

  cipher gcm auth encrypt:
     93 FE 7D 9E 9B FD 10 34 8A 56 06 E5 CA FA 73 54

  cipher gcm auth tag:
     00 32 A1 DC 85 F1 C9 78 69 25 A2 E7 1D 82 72 DD

  cipher gcm auth decrypt:
     C3 B3 C4 1F 11 3A 31 B7 3D 9A 5C D4 32 10 30 69
```