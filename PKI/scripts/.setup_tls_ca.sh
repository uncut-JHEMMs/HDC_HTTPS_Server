#!/usr/bin/env bash <- this allows env to find bash, it's better than just /bin/bash
#Creates and sets up a TLS CA from a configuration file
#etc/tls-ca-conf, and bundles the created key with the root certificate in
#a PEM bundle

#mostly lifted from pki-tutorial.readthedocs.io/en/latest/advanced

#Create directories
mkdir -p ca/tls-ca/private ca/tls-ca/db crl certs
chmod 700 ca/tls-ca/private

#Create database
cp /dev/null ca/tls-ca/db/tls-ca.db
cp /dev/null ca/tls-ca/db/tls-ca.db.attr
echo 01 > ca/tls-ca/db/tls-ca.crt.srl
echo 01 > ca/tls-ca/db/tls-ca.crl.srl

#Create CA Request
openssl req -new \
    -config etc/tls-ca.conf \
    -out ca/tls-ca.csr \
    -keyout ca/tls-ca/private/tls-ca.key

#Create CA Certificate
openssl ca \
    -config etc/root-ca.conf \
    -in ca/tls-ca.csr \
    -out ca/tls-ca.crt \
    -extensions signing_ca_ext

#Create CRL
openssl ca -gencrl \
    -config etc/tls-ca.conf \
    -out crl/tls-ca.crl

#Create PEM bundle
cat ca/tls-ca.crt ca/root-ca.crt > \
    ca/tls-ca-chain.pem
