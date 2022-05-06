#!/usr/bin/env bash <- this allows env to find bash, it's better than just /bin/bash
#Creates and sets up a document-signing CA from a configuration file 
#etc/docsign-ca-conf, and bundles the created key with the root certificate in
#a PEM bundle

#mostly lifted from pki-tutorial.readthedocs.io/en/latest/advanced, with some small
#changes

#Create directories
mkdir -p ca/docsign-ca/private ca/docsign-ca/db crl certs
chmod 700 ca/docsign-ca/private
#Create databases
cp /dev/null ca/docsign-ca/db/docsign-ca.db
cp /dev/null ca/docsign-ca/db/docsign-ca.db.attr
echo 01 > ca/docsign-ca/db/docsign-ca.crt.srl
echo 01 > ca/docsign-ca/db/docsign-ca.crl.srl
#Create CA Request and key
openssl req -new \
    -config etc/docsign-ca.conf \
    -out ca/docsign-ca.csr \
    -keyout ca/docsign-ca/private/docsign-ca.key

#Create CA Certificate
openssl ca -config etc/root-ca.conf \
    -in ca/docsign-ca.csr \
    -out ca/docsign-ca.crt \
    -extensions signing_ca_ext

#Create initial CRL
openssl ca -gencrl \
    -config etc/root-ca.conf \
    -out crl/root-ca.crl

#Create PEM Bundle
cat ca/docsign-ca.crt ca/root-ca.crt > \
    ca/docsign-ca-chain.pem
