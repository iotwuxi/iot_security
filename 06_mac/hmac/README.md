# hmac

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  md info setup, name: SHA256, digest size: 32

  md hmac-sha-256 mac:
     5B DC C1 46 BF 60 75 4E 6A 04 24 26 08 95 75 C7
     5A 00 3F 08 9D 27 39 83 9D EC 58 B9 64 EC 38 43
```