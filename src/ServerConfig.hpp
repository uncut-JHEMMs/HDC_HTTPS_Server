#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP
/*
 * This file holds the definitions to types and functions related to starting
 * a libhttpserver with customized behavior
 */
#include <iostream>
#include <fstream>
#include <thread> //hardware concurrency
#include <regex> //isNumber
#include <string>
#include <cstdlib>
#include <sstream> //parse line, toString
#include <vector>
#include <httpserver.hpp>
#include "Parser.hpp"

//used for finding reasonable thread limit, arbitrary here, tune it later
#define REASONABLE_COEFFICIENT 4
//environment variable names
//put all of these in a container so we can just iterate over all of them
#define portKey "UTOPIA_PORT"
#define TPSizeKey "UTOPIA_TPS"
#define maxConnectKey "UTOPIA_CONNECTIONS"
#define ipConsKey "UTOPIA_IP_CON_CONNECTIONS"
#define tlsKey "UTOPIA_TLS"
#define timeoutKey "UTOPIA_TIMEOUT"
#define dsKey "UTOPIA_DUAL_STACK"
#define tlsCert "UTOPIA_TLS_CERT"
#define tlsCertKey "UTOPIA_TLS_KEY"

/*
 * A ServerStarter contains fields that correspond to the startup options that
 * our server will support, as well as the functions that parse those values 
 * from the different input sources according to the project spec 
 */
struct ServerConfig{
    //start with supporting these options, add more later
    unsigned int portNumber;
    //maximum number of concurrent connections to accept
    unsigned int maxConnections;
    //number of worker threads
    unsigned int threadPoolSize;
    //max connections per unique IP
    unsigned int maxConnectionsPerIP;
    //need connection timeout
    unsigned int connectionTimeout;
    //https
    bool useHTTPS;
    //whether or not to support ipv6 in addition to ipv6, I don't think
    //ipv6 alone is ever a good idea
    bool dualStack;
    //whether or not the server should block this thread
    bool doesBlock;
    //https needs these
    std::string pathToKeyFile;
    std::string pathToCertFile;
    
    /*A reasonable number of threads is a coefficient of the available number
     * of physical threads. Remember that other stuff will probably be running
     * on this computer too, we don't want to try to grab everything
     */
    unsigned int getReasonableThreadMax();
    
    //populate all members from config file
    void populateAllFromConfigFile(std::string& fileName);
    void populateAllFromEnv();


    //just have methods for populate each, then a master that calls all
    //port from config
    bool populatePortFromConfig(ConfigFileParser& fileParser);
    //port from Env
    void populatePortFromEnv();
    //max connections
    bool populateMaxConnectFromConfig(ConfigFileParser& fileParser);
    //max connect from Env
    void populateMaxConnectionsFromEnv();
    //ip con connections
    bool populateIPConConnectionsFromConfig(ConfigFileParser& fileParser);
    //ip con connections env
    void populateIPConConnectionsFromEnv();
    //Threadpool size
    bool populateTPSFromConfig(ConfigFileParser& fileParser);
    //TPS from env
    void populateTPSFromEnv();
    //timeout
    bool populateTimeoutFromConfig(ConfigFileParser& fileParser);
    //Timeout from env
    void populateTimeoutFromEnv();
    //use HTTPS
    bool populateHTTPSFromConfig(ConfigFileParser& fileParser);
    //HTTPS from env
    void populateHTTPSFromEnv();
    //HTTPS cert
    bool populateTLSCertFromConfig(ConfigFileParser& fileParser);
    //cert from env
    bool populateTLSCertFromEnv();
    //HTTPS key
    bool populateTLSKeyFromConfig(ConfigFileParser& fileParser);
    //key from env
    bool populateTLSKeyFromEnv();
    //Dual Stack
    bool populateDSFromConfig(ConfigFileParser& fileParser);
    //DS from env
    void populateDSFromEnv();
    /*
        returns a string representation of the current config
    */
    std::string toString();
    //default constructor assigns blank values
    ServerConfig();
    //parameterized constructor takes a string that reporesents the path to 
    //a configuration file
    ServerConfig(std::string pathToConfigFile);
    //helper
    bool convertToBool(const std::string& value);
    //TODO: move these to parser only
    bool isValidNumber(std::string input) const;
    //check that the port number is valid, 1-65535
    bool isPortValid(unsigned int port);


};
#endif
