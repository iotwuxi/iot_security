#!/bin/bash

ORG="iotwuxi"
CA_CN="iotwuxi-Root-CA"
DOMAIN="iotwuxi.org"
CERT_DIR="certs"
HEADFILE="ca_cert.h"
ECC_CURV="secp256r1"

rm -rf $CERT_DIR
mkdir $CERT_DIR
cd $CERT_DIR

# 生成 CA 密钥对
echo "  [cert] generate ca ecc keypiar($ECC_CURV)."
gen_key type=ec ec_curve=$ECC_CURV filename=ca_privkey.pem format=pem > /dev/null

# 生成根证书
echo "  [cert] generate ca cert."
cert_write selfsign=1 issuer_key=ca_privkey.pem md=SHA256 issuer_name=CN=$CA_CN,O=$ORG,C=CN output_file=ca_cert.pem not_before=20180101000000 not_after=20231231115959 is_ca=1 serial=16011 > /dev/null

# 生成服务器密钥对
echo "  [cert] generate server ecc keypiar($ECC_CURV)."
gen_key type=ec ec_curve=$ECC_CURV filename=srv_privkey.pem format=pem > /dev/null

# 生成服务器 csr
echo "  [cert] generate server csr."
cert_req filename=srv_privkey.pem output_file=srv_cert.req subject_name=CN=$DOMAIN,O=$ORG,C=CN > /dev/null

# 生成服务器证书
echo "  [cert] generate server cert."
cert_write request_file=srv_cert.req md=SHA256 issuer_key=ca_privkey.pem issuer_name=CN=$CA_CN,O=$ORG,C=CN output_file=srv_cert.pem not_before=20180101000000 not_after=20191231115959 serial=16012 > /dev/null

# 将 pem 格式证书转换为 der 格式，写入到 $HEADFILE
pem2der filename=ca_cert.pem output_file=ca_cert.der > /dev/null
xxd -g 1 -i -u ca_cert.der >> $HEADFILE

echo "  [cert] generate success, save to $HEADFILE"