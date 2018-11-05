# tls

## 启动 tls server
```
$ openssl s_server --state -cert srv_cert.pem -key srv_privkey.pem -CAfile ca_cert.pem -port 442 -cipher ECDHE-ECDSA-AES256-GCM-SHA384 -WWW ./
```

## 启动 tls client
```
$ mkdir build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## tls client 输出
```
  tls client use native_posix board.

  . Seeding the random number generator ...  ok
  . Setting up the SSL/TLS structure ...  ok
  . Connecting to iotwuxi.org:442... ok
  . Performing the SSL/TLS handshake ... ok
  > Write to server: 28 bytes written

GET /index.html HTTP/1.0



  > Read from server: 198 bytes read


HTTP/1.0 200 ok
Content-type: text/html


<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>TLS Server</title>
</head>
<body>
    <h1>This is TLS Server</h1>
</body>
</html>


 ok
  . Closing the connection ... done
```

## tls server 输出
```
Using default temp DH parameters
ACCEPT
SSL_accept:before SSL initialization
SSL_accept:before SSL initialization
SSL_accept:SSLv3/TLS read client hello
SSL_accept:SSLv3/TLS write server hello
SSL_accept:SSLv3/TLS write certificate
SSL_accept:SSLv3/TLS write key exchange
SSL_accept:SSLv3/TLS write server done
SSL_accept:SSLv3/TLS write server done
SSL_accept:SSLv3/TLS read client key exchange
SSL_accept:SSLv3/TLS read change cipher spec
SSL_accept:SSLv3/TLS read finished
SSL_accept:SSLv3/TLS write change cipher spec
SSL_accept:SSLv3/TLS write finished
FILE:index.html
ACCEPT
```
