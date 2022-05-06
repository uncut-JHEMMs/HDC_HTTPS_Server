#!/bin/less
Utopia Server - A customizable http server with logging and performance monitoring

The Utopia Server is an http server library built on top of libhttpserver, which is itself built on top of libmicrohttpd. Utopia Server enhances libhttpserver by providing fast re-configuration of the server via either a config file or operating system environment variables, verbose logging of accesses to the server as well as server internal conditions, and a suite of performance monitoring tools to aid in diagnosing issues in server performance.

This is a living document as the server itself is still under development.

STARTUP OPTIONS

	Navigate into this directory and run make to compile the project

	To run the server, use the executable serverStart that gets created in this directory:
	./serverStart

	When run with no arguments, the server will attempt to start by using environment variables (see environment variables below)

CONFIGURATION VARIABLES

	Utopia Server runs by searching for a configuration, either from a config file or from environment varibales by supplying no additional arguments when running the executable. The expected names of the variables are listed below with valid values, as well as an explanation of what each does:

	UTOPIA_PORT
		This variable controls the port that the server starts on. Valid values are 1-65535. Attempting to start the server with a port that is in use will cause startup to fail.

	UTOPIA_TIMEOUT
		This variable controls the number of seconds that the server will wait on new traffic from an accepted connection before closing the connection.

	UTOPIA_DUAL_STACK
		This variable controls whether or not the server maps ipv4 addresses into the ipv6 address space. If this is 0, the server is accessible via ipv4 addresses. If this is 1, the server is only accessible via ipv6 addresses.

	UTOPIA_TPS
		This varible controls the size of the worker thread pool. The workers in the pool are used to service http requests. In general, this number being larger means better performance, but setting it too high will cause thread contention and make performance worse.

	UTOPIA_CONNECTIONS
		This variable controls the maximum number of concurrent connections the server will support before it begins refusing connections.

	UTOPIA_IP_CON_CONNECTIONS
		This variable controls the maximum number of concurrent connections the server will support from a single IP address. If this number is very close to UTOPIA_CONNECTIONS, it's possible one connection that makes multiple requests (for instance, for an html file as well as for images in the html), then a single connection may be able to monopolize all of the server's available connections.

	UTOPIA_TLS
		This variable controls whether or not the server uses HTTPS. If this option is set, a certificate and key file must be supplied and https must be used to connect to the server.

	UTOPIA_TLS_CERT
		This variable is the path to the server TLS certificate file to use with HTTPS.

	UTOPIA_TLS_KEY
		This variable is the path to the server TLS key file to use with HTTPS.

MODIFYING CONFIGS

CREATING NEW KEYS

PERFORMANCE MONITORING

LOGGING

