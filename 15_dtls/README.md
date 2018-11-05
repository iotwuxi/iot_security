# dtls

## 启动 dtls server
```
$ openssl s_server -state -nocert -dtls1_2 -port 4432 -cipher PSK-AES128-CCM8 -psk_hint Client_identity -psk 000102030405060708090a0b0c0d0e0f
```

## 启动 dtls client
```
$ mkdir build && cd build
$ cmake -DBOARD=native_posix ..
$ make && make run
```

## dtls client 输出
```
  . Seeding the random number generator... ok
  . Setting up the SSL/TLS structure... ok
  . Connecting to iotwuxi.org:4432... ok
  . Performing the SSL/TLS handshake... ok
  > Write to server: 11 bytes written

Echo this

  > Read from server: 11 bytes read

Echo this

 . Closing the connection ... done
```

## dtls server 输出
```
Using default temp DH parameters
ACCEPT
SSL_accept:before SSL initialization
SSL_accept:before SSL initialization
SSL_accept:SSLv3/TLS read client hello
SSL_accept:DTLS1 write hello verify request
SSL_accept:DTLS1 write hello verify request
SSL_accept:SSLv3/TLS read client hello
SSL_accept:SSLv3/TLS write server hello
SSL_accept:SSLv3/TLS write key exchange
SSL_accept:SSLv3/TLS write server done
SSL_accept:SSLv3/TLS write server done
SSL_accept:SSLv3/TLS read client key exchange
SSL_accept:SSLv3/TLS read change cipher spec
SSL_accept:SSLv3/TLS read finished
SSL_accept:SSLv3/TLS write change cipher spec
SSL_accept:SSLv3/TLS write finished
-----BEGIN SSL SESSION PARAMETERS-----
MIGJAgEBAgMA/v0EAsCpBCB65LU9RBY7wI7ko6S57KndinD2AXUpNccQ4Nf7LMJ3
WgQwA4x7aL/66esOMfeOs1t3hTL02ZE2+S51Wd4Z6m1h7Bw59cnn18JxKD2VI7y+
CTq0oQYCBFuXOzyiBAICHCCkBgQEAQAAAKgRBA9DbGllbnRfaWRlbnRpdHk=
-----END SSL SESSION PARAMETERS-----
Shared ciphers:PSK-AES128-CCM8
CIPHER is PSK-AES128-CCM8
Secure Renegotiation IS supported

Echo this

```
