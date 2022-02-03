#!/usr/bin/env bash <- this allows env to find bash, it's better than just /bin/bash
#sets up a key to use as a CA, creates a CSR, then signs the CSR with the CA's key

#just put the keys here for now
#mkdir ~/certs
#cd ~/certs
#generate a key for the certificate authority
openssl genrsa -out ca.key 2048
#use the key to make a certificate
openssl req -new -x509 -key ca.key -out ca.crt








##########CSR PART################################
#now generate a request
openssl genrsa -out example.org.key 2048
#can extract the public key from the private key with the below:
#openssl rsa -in example.org.key -pubout -out example.org.pubkey
#openssl rsa -in example.org.pubkey -pubin -noout -text

#generate a CSR
openssl req -new -key example.org.key -out example.org.csr
#you go through the whole riggamarole, or you can supply a config file to do it
#(look at gist github)


#how to sign a certificate with our CA key
#openssl x509 -req -in example.org.csr -CA ca.crt -CAkey ca.key -CAcreateserial \
#-out exampl.org.crt
