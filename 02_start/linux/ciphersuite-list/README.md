# ciphersuite-list

## 全功能套件

### 编译和运行
```
# 全功能套件
$ mkdir -p build && cd build
$ cmake ..
$ make
$ ./ciphersuite-list
```

### 用例输出
```
  Available Ciphersuite:
  [001] TLS-ECDHE-ECDSA-WITH-AES-256-GCM-SHA384
  [002] TLS-ECDHE-RSA-WITH-AES-256-GCM-SHA384
  [003] TLS-DHE-RSA-WITH-AES-256-GCM-SHA384
  [004] TLS-ECDHE-ECDSA-WITH-AES-256-CCM
  [005] TLS-DHE-RSA-WITH-AES-256-CCM
  [006] TLS-ECDHE-ECDSA-WITH-AES-256-CBC-SHA384
  [007] TLS-ECDHE-RSA-WITH-AES-256-CBC-SHA384
  [008] TLS-DHE-RSA-WITH-AES-256-CBC-SHA256
  [009] TLS-ECDHE-ECDSA-WITH-AES-256-CBC-SHA
  // 省略部分输出内容
```

## 部分功能套件

### 编译和运行
```
$ find . -iname '*cmake*' -not -name CMakeLists.txt -exec rm -rf {} +
$ CFLAGS="-I$MBEDTLS_BASE/configs -DMBEDTLS_CONFIG_FILE='<config-mini-tls1_1.h>'" cmake ..
$ make
```

### 用例输出
```
  Available Ciphersuite:
  [001] TLS-RSA-WITH-AES-256-CBC-SHA256
  [002] TLS-RSA-WITH-AES-256-CBC-SHA
  [003] TLS-RSA-WITH-AES-128-CBC-SHA256
  [004] TLS-RSA-WITH-AES-128-CBC-SHA
  [005] TLS-RSA-WITH-3DES-EDE-CBC-SHA
```