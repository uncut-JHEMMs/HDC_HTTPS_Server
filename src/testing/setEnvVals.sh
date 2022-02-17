#!/bin/bash
#set Env variables in the shell that's running the tests
UTOPIA_PORT=8000
UTOPIA_TPS=20
UTOPIA_CONNECTIONS=5
UTOPIA_TLS=1
UTOPIA_TLS_CERT=../PKI/certs/mycert.crt
UTOPIA_TLS_KEY=../PKI/ca/mykey.pem

export UTOPIA_PORT
export UTOPIA_TPS
export UTOPIA_CONNECTIONS
export UTOPIA_TLS
export UTOPIA_TLS_CERT
export UTOPIA_TLS_KEY
