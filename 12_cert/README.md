# cert

## 编译和运行
```
$ mkdir -p build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## 用例输出
```
  . Loading the certificate(s) ... ok

  . Verify requested for (Depth 1) ... ok
      cert. version     : 3
      serial number     : 01
      issuer name       : CN=CA, O=security, C=china
      subject name      : CN=CA, O=security, C=china
      issued  on        : 2019-01-01 00:00:00
      expires on        : 2022-01-01 00:00:00
      signed using      : ECDSA with SHA256
      EC key size       : 256 bits
      basic constraints : CA=true

  . Verify requested for (Depth 0) ... ok
      cert. version     : 3
      serial number     : 01
      issuer name       : CN=CA, O=security, C=china
      subject name      : CN=Bob, O=security, C=china
      issued  on        : 2019-01-01 00:00:00
      expires on        : 2022-01-01 00:00:00
      signed using      : ECDSA with SHA256
      EC key size       : 256 bits
      basic constraints : CA=false
```