# 使能转发
sysctl -w net.ipv4.conf.all.forwarding=1

# 删除 nat 表中的所有规则
iptables -t nat -F

# 添加 nat 规则
iptables -t nat -A POSTROUTING -j MASQUERADE -s 192.0.2.1

# dns
service dnsmasq restart

# 查看 iptables nat 规则
iptables -L -n -t nat
