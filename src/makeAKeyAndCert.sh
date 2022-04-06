#these two commands are necessary to generate a private key and then a certificate from it
#also need to generate a csr and crl for the first sprint
openssl genrsa -out newkey.pem 4096

openssl req -key newkey.pem -new -x509 -days 365 -out newcert.crt
