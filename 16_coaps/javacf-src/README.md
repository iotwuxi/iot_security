# Java CF示例

## 概述
- 包含客户端和服务器
- 仅包含PSK密钥交换
- 加密套件支持

## coaps server
coaps-server文件夹中包括coaps服务器
- PSK为password
- Identity为identity
- DTLS端口号为5684
``` java
InMemoryPskStore pskStore = new InMemoryPskStore();
pskStore.setKey("identity", "password".getBytes());
```

```t

```

