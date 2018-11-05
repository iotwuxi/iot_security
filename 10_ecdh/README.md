# ecdh

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  . setup rng ... ok

  . select ecp group SECP256R1 ... ok
  1. ecdh client generate public parameter:
     04 18 66 03 B8 81 CE 88 FA AE 11 D3 FC 05 40 63
     24 36 F7 3A 98 98 72 E9 17 53 2C 3B BD CC E1 60
     2A 2B 1C 23 FE F4 23 66 EE F4 B0 E1 9C FF 95 DD
     3A 52 84 6F 97 B5 01 63 AA CC 4B 7E 10 B4 8D 85
     60
  2. ecdh server generate public parameter:
     04 68 9B 3C 78 7C D6 99 04 66 F0 44 C1 AC 23 AE
     37 A8 1E 89 5A 34 A7 9C 78 8F F3 D3 23 B0 0F DE
     53 24 77 9A 57 DF 57 CC 18 E4 67 7E 2D 99 67 B1
     09 91 90 28 9D 84 DB DC C5 BE A1 13 12 C4 FB 92
     50
  3. ecdh client generate secret:
     D1 C9 8F 94 53 AD 3A 1E 46 AE FD BC 7E BA 21 57
     42 B5 58 A0 34 50 82 E9 C7 08 B6 1F 29 9F DB BF
  4. ecdh server generate secret:
     D1 C9 8F 94 53 AD 3A 1E 46 AE FD BC 7E BA 21 57
     42 B5 58 A0 34 50 82 E9 C7 08 B6 1F 29 9F DB BF
  5. ecdh checking secrets ... ok
```