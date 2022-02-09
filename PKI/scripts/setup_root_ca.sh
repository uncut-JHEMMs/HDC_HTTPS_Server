#!/usr/bin/env bash <- this allows env to find bash, it's better than just /bin/bash
#Creates and sets up a root CA, from a configuration file ../etc/root-ca-conf
#mostly lifted from pki-tutorial.readthedocs.io/en/latest/advanced, with some small
#changes

#Create directories 
mkdir -p ca/root-ca/private ca/root-ca/db crl certs
chmod 700 ca/root-ca/private
#Create databases
cp /dev/null ca/root-ca/db/root-ca.db
cp /dev/null ca/root-ca/db/root-ca.db.attr
echo 01 > ca/root-ca/db/root-ca.crt.srl
echo 01 > ca/root-ca/db/root-ca.crl.srl
#Create CA Request and key
openssl req -new \
    -config etc/root-ca.conf \
    -out ca/root-ca.csr \
    -keyout ca/root-ca/private/root-ca.key

#create CA Certificate and self sign
openssl ca -selfsign \
    -config etc/root-ca.conf \
    -in ca/root-ca.csr \
    -out ca/root-ca.crt \
    -extensions root_ca_ext \
    -enddate 20301231235959Z
#Creat initial CRL
openssl ca -gencrl \
    -config etc/root-ca.conf \
    -out crl/root-ca.crl
