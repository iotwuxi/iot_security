# performance

## 编译和运行
```
# 串口
$ minicom -b 115200 -D /dev/ttyACM0

# 下载
$ mkdir -p build && cd build
$ cmake -DBOARD=nucleo_f429zi ..
$ make flash
```

## 用例输出
```
***** Booting Zephyr OS 1.12.0 *****

  SHA-1                    :       3775 KiB/s,         46 cycles/byte
  SHA-256                  :       1962 KiB/s,         89 cycles/byte
  SHA-512                  :        704 KiB/s,        249 cycles/byte
  AES-CBC-128              :       1372 KiB/s,        128 cycles/byte
  AES-CBC-192              :       1207 KiB/s,        145 cycles/byte
  AES-CBC-256              :       1078 KiB/s,        163 cycles/byte
  AES-GCM-128              :        485 KiB/s,        363 cycles/byte
  AES-GCM-192              :        462 KiB/s,        381 cycles/byte
  AES-GCM-256              :        441 KiB/s,        399 cycles/byte
  AES-CCM-128              :        591 KiB/s,        298 cycles/byte
  AES-CCM-192              :        528 KiB/s,        333 cycles/byte
  AES-CCM-256              :        477 KiB/s,        369 cycles/byte
  AES-CMAC-128             :       1079 KiB/s,        163 cycles/byte
  AES-CMAC-192             :        972 KiB/s,        181 cycles/byte
  AES-CMAC-256             :        884 KiB/s,        199 cycles/byte
  CTR_DRBG (NOPR)          :       1128 KiB/s,        155 cycles/byte
  CTR_DRBG (PR)            :        740 KiB/s,        237 cycles/byte
  RSA-2048                 :   38.70  public/s       4040 heap bytes
  RSA-2048                 :    1.00 private/s       8232 heap bytes
  DHE-2048                 :    0.30 handshake/s    10196 heap bytes
  DH-2048                  :    0.40 handshake/s    11164 heap bytes
  DHE-3072                 :    0.20 handshake/s    15060 heap bytes
  DH-3072                  :    0.20 handshake/s    15392 heap bytes

# 最优性能
  ECDSA-secp521r1          :    2.80 sign/s         14068 heap bytes
  ECDSA-secp384r1          :    4.70 sign/s         10740 heap bytes
  ECDSA-secp256r1          :    7.50 sign/s          4448 heap bytes
  ECDSA-secp224r1          :   11.00 sign/s          3952 heap bytes
  ECDSA-secp192r1          :   14.80 sign/s          3776 heap bytes
  ECDSA-secp521r1          :    0.90 verify/s       14672 heap bytes
  ECDSA-secp384r1          :    1.40 verify/s       11276 heap bytes
  ECDSA-secp256r1          :    2.30 verify/s        5456 heap bytes
  ECDSA-secp224r1          :    3.30 verify/s        4872 heap bytes
  ECDSA-secp192r1          :    4.40 verify/s        4556 heap bytes
  ECDHE-secp521r1          :    0.90 handshake/s    14044 heap bytes
  ECDHE-secp384r1          :    1.50 handshake/s    10756 heap bytes
  ECDHE-secp256r1          :    2.40 handshake/s     4880 heap bytes
  ECDHE-secp224r1          :    3.50 handshake/s     4396 heap bytes
  ECDHE-secp192r1          :    4.80 handshake/s     4152 heap bytes
  ECDHE-Curve25519         :    1.90 handshake/s     1208 heap bytes
  ECDH-secp521r1           :    1.20 handshake/s     7392 heap bytes
  ECDH-secp384r1           :    1.90 handshake/s     5576 heap bytes
  ECDH-secp256r1           :    3.20 handshake/s     2336 heap bytes
  ECDH-secp224r1           :    4.70 handshake/s     2228 heap bytes
  ECDH-secp192r1           :    6.50 handshake/s     1988 heap bytes
  ECDH-Curve25519          :    3.70 handshake/s     1028 heap bytes

# 最少内存
  ECDSA-secp521r1          :    0.20 sign/s          3144 heap bytes
  ECDSA-secp384r1          :    0.30 sign/s          2352 heap bytes
  ECDSA-secp256r1          :    0.60 sign/s          1660 heap bytes
  ECDSA-secp224r1          :    0.70 sign/s          1488 heap bytes
  ECDSA-secp192r1          :    0.90 sign/s          1364 heap bytes
  ECDSA-secp521r1          :    0.20 verify/s        3992 heap bytes
  ECDSA-secp384r1          :    0.20 verify/s        2952 heap bytes
  ECDSA-secp256r1          :    0.40 verify/s        2188 heap bytes
  ECDSA-secp224r1          :    0.40 verify/s        1924 heap bytes
  ECDSA-secp192r1          :    0.50 verify/s        1728 heap bytes
  ECDHE-secp521r1          :    0.20 handshake/s     2996 heap bytes
  ECDHE-secp384r1          :    0.20 handshake/s     2264 heap bytes
  ECDHE-secp256r1          :    0.40 handshake/s     1708 heap bytes
  ECDHE-secp224r1          :    0.40 handshake/s     1544 heap bytes
  ECDHE-secp192r1          :    0.50 handshake/s     1364 heap bytes
  ECDHE-Curve25519         :    1.90 handshake/s     1208 heap bytes
  ECDH-secp521r1           :    0.20 handshake/s     3052 heap bytes
  ECDH-secp384r1           :    0.30 handshake/s     2288 heap bytes
  ECDH-secp256r1           :    0.60 handshake/s     1680 heap bytes
  ECDH-secp224r1           :    0.70 handshake/s     1548 heap bytes
  ECDH-secp192r1           :    0.90 handshake/s     1380 heap bytes
  ECDH-Curve25519          :    3.60 handshake/s     1020 heap bytes
```
