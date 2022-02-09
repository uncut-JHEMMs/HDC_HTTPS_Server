#!/usr/bin/env bash <- this allows env to find bash, it's better than just /bin/bash
#Creates and sets up a server-signing CA from a configuration file
#etc/serversign-ca-conf, and bundles the created key with the root certificate in
#a PEM bundle

#mostly lifted from pki-tutorial.readthedocs.io/en/latest/advanced, with some small
#changes

#Create directories
mkdir -p ca/serversign-ca/private ca/serversign-ca/db crl certs
chmod 700 ca/serversign-ca/private
#Create databases
cp /dev/null ca/serversign-ca/db/serversign-ca.db
cp /dev/null ca/serversign-ca/db/serversign-ca.db.attr
echo 01 > ca/serversign-ca/db/serversign-ca.crt.srl
echo 01 > ca/serversign-ca/db/serversign-ca.crl.srl
#Create CA Request and key
openssl req -new \
    -config etc/serversign-ca.conf \
    -out ca/serversign-ca.csr \
    -keyout ca/serversign-ca/private/serversign-ca.key

#Create CA Certificate
openssl ca -config etc/root-ca.conf \
    -in ca/serversign-ca.csr \
    -out ca/serversign-ca.crt \
    -extensions signing_ca_ext

#Create initial CRL
openssl ca -gencrl \
    -config etc/serversign-ca.conf \
    -out crl/serversign-ca.crl

#Create PEM Bundle
cat ca/serversign-ca.crt ca/root-ca.crt > \
    ca/serversign-ca-chain.pem

