# CoAPs

## coaps server
`coaps-server` 文件夹中包括 `coaps` 服务器
- `PSK` 为 `password`
- `Identity` 为 `identity`
- `DTLS` 端口号为 `5684`

``` java
InMemoryPskStore pskStore = new InMemoryPskStore();
pskStore.setKey("identity", "password".getBytes());
```

## coap client
```
$ mkdir build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run

# 输出
  . Seeding the random number generator ... ok
  . Setting up the SSL/TLS structure ... ok
  . Connecting to 192.168.1.107:5684 ... ok
  . Performing the SSL/TLS handshake ... ok
  > Set coap packet ...  ok
  > Write to server ...  ok
  > Read from server ...  ok
  > Parse coap packet ...  ok

  . [COAP] Response Code: 69
  . [COAP] Response Message id: 1
  . [COAP] Response Payload: Hello Security!
```

