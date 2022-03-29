#ifndef SERVERSTARTER_HPP
#define SERVERSTARTER_HPP
/*
 * This file holds the definitions to types and functions related to starting
 * a libhttpserver with customized behavior
 */
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
#define blockKey "UTOPIA_BLOCK"
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
    unsigned short portNumber;
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
    //we need a parse member here

    /*A reasonable number of threads is a coefficient of the available number
     * of physical threads. Remember that other stuff will probably be running
     * on this computer too, we don't want to try to grab everything
     */
    unsigned int getReasonableThreadMax(){
        return REASONABLE_COEFFICIENT * std::thread::hardware_concurrency();
    }

    bool isValidNumber(std::string input) const{
        for (char c : input){
            if (!std::isdigit(c)) return false;
        }
        return true;
        }
    //check that the port number is valid, 1-65535
    bool isPortValid(unsigned int port){
        if (port> 0 && port < 65536) return true;
        return false;
    }

    //populate all members from config file
    void populateAllFromConfigFile(std::string& fileName){
        try{
            populatePortFromConfig(fileName);
            populateMaxConnectFromConfig(fileName);
            populateTPSFromConfig(fileName);
            populateTimeoutFromConfig(fileName);
	    //TODO: add the last 2 fields
            populateHTTPSFromConfig(fileName);
            populateTLSCertFromConfig(fileName);
            populateTLSKeyFromConfig(fileName);
        }
        catch(const char* c){
            //just propagate the error up
            throw(c);
        }
    }

    //just have methods for populate each, then a master that calls all
    bool populatePortFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser =
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{portKey};
            std::string sPort = fileParser->getValueFromKey(val);
            //set the member
            portNumber = fileParser->convertToUInt(sPort);
        }
        catch(const char* c){
            throw("Error assigning port from config file. Check key and value.");
        }
        //This is not working right now
        const int limit = 65535;
        if (portNumber > limit){
            throw("Port Number is outside valid range. Must be below 65536");
        }
        return true;
        //unique pointer falls out of scope here
    }

    //just have methods for populate each, then a master that calls all
    bool populateMaxConnectFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{maxConnectKey};
            std::string sMC = fileParser->getValueFromKey(val);
            //set the member
            maxConnections = fileParser->convertToUInt(sMC);
        }
        catch(const char* c){
            throw("Error assigning max connections from config file. Check key and value.");
            //return false;
        }
        return true;
        //unique pointer falls out of scope here
    }

    //Threadpool size
    bool populateTPSFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{TPSizeKey};
            std::string sTPS = fileParser->getValueFromKey(val);
            //set the member
            threadPoolSize = fileParser->convertToUInt(sTPS);
            if (threadPoolSize > getReasonableThreadMax()){
                std::cout << "Your threadpool size is significantly higher than your machine's thread count. This is not recommended." << std::endl;
            }
        }
        catch(const char* c){
            throw("Error assigning TPS from config file. Check key and value.");

        }
        return true;
        //unique pointer falls out of scope here
    }

    //timeout
    bool populateTimeoutFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{timeoutKey};
            std::string sTPS = fileParser->getValueFromKey(val);
            //set the member
            connectionTimeout = fileParser->convertToUInt(sTPS);
        }
        catch(const char* c){
            throw("Error assigning timeout from config file. Check key and value.");

        }
        return true;
        //unique pointer falls out of scope here
    }

    //use HTTPS
    bool populateHTTPSFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{tlsKey};
            std::string sHTTPS = fileParser->getValueFromKey(val);
            //set the member
            useHTTPS = fileParser->convertToBool(sHTTPS);
        }
        catch(const char* c){
            throw("Error assigning TLS from config file. Check key and value.");
            return false;
        }
        return true;
        //unique pointer falls out of scope here
    }

    //HTTPS cert
    bool populateTLSCertFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{tlsCert};
            //no conversion needed
            pathToCertFile = fileParser->getValueFromKey(val);
            
        }
        catch(const char* c){
            throw("Error assigning cert from config file. Check key and value.");
            return false;
        }
        return true;
        //unique pointer falls out of scope here
    }
    
    //HTTPS key
    bool populateTLSKeyFromConfig(std::string& configFile){
        //make a parser (abstract out)
        std::unique_ptr<ConfigFileParser> fileParser = 
            std::make_unique<ConfigFileParser>(configFile);
        //have it get the value
        try{
            std::string val{tlsCertKey};
            //no conversion needed
            pathToKeyFile = fileParser->getValueFromKey(val);
        }
        catch(const char* c){
            throw("Error assigning TLS Key from config file. Check key and value.");
            return false;
        }
        return true;
        //unique pointer falls out of scope here
    }

    //populate the data members from environment variables. Use the names defined above in this file.
    bool populateFromEnv(){
        //if any entry is invalid, we stop parsing, log the error and terminate
        //get each of the environment variables in the list
        char* valc = getenv(portKey);
        std::string vals(valc); 
        if (isValidNumber(vals)){
            portNumber = stoi(vals);
        }
        valc = getenv(timeoutKey);
        vals = valc;
        if (isValidNumber(vals)){
            connectionTimeout = stoi(vals);
        }
        valc = getenv(blockKey);
        vals = valc;
        if (isValidNumber(vals)){
            if (vals == "0") doesBlock = false;
            else doesBlock = true;
        }
        valc = getenv(dsKey);
        vals = valc;
        if (isValidNumber(vals)){
            if (vals == "0") dualStack = false;
            else dualStack = true;
        }
        valc = getenv(TPSizeKey);
        vals = valc;
        if (isValidNumber(vals)){
            threadPoolSize = stoi(vals);
        }
        valc = getenv(maxConnectKey);
        vals = valc; 
        if (isValidNumber(vals)){
            maxConnections = stoi(vals);
        }
        valc = getenv(ipConsKey);
        vals = valc; 
        if (isValidNumber(vals)){
            maxConnectionsPerIP = stoi(vals);
        }
        valc = getenv(tlsKey);
        vals = valc; 
        if (isValidNumber(vals)){
            if (vals == "0") useHTTPS = false;
            else useHTTPS = true;
        }
        //strings at the bottom here
        valc = getenv(tlsCert);
        vals = valc;
        pathToCertFile = vals;

        valc = getenv(tlsCertKey);
        vals = valc;
        pathToKeyFile = vals; 
        return true;
    }
    /*
        returns a string representation of the current config
    */
    std::string toString(){
        std::stringstream stream;
        stream << "Server configuration:" << std::endl;
        //port number
        stream << "Port Number: " << portNumber << std::endl;
        //max connections
        stream << "Maximum Concurrent Connections: " << maxConnections << std::endl;
        //max connections per IP
        stream << "Maximum Connections Per IP: " << maxConnectionsPerIP << std::endl;
        //timeout
        stream << "Timeout (in seconds): " << connectionTimeout << std::endl;
        //tps
        stream << "Threadpool size: " << threadPoolSize << std::endl;
        //https
        stream << "HTTPS: " << useHTTPS << std::endl;
        //dual stack
        stream << "Map IPv4 into IPv6: " << dualStack << std::endl;
        //location of certificate
        stream << "SSL Certificate: " << pathToCertFile << std::endl;
        //location of key
        stream << "SSL Key: " << pathToKeyFile;
        std::string res = stream.str();
        return res;
    }

    //default constructor assigns blank values
    ServerConfig(){
        portNumber = 0;
        doesBlock = true;
        dualStack = true;
        connectionTimeout = 0;
        maxConnections = 0;
        threadPoolSize = 0;
        //reasonableThreadMax = 0;
        maxConnectionsPerIP = 0;
        useHTTPS = false;

    }
    //parameterized constructor takes a string that reporesents the path to 
    //a configuration file
    ServerConfig(std::string pathToConfigFile){
        populateAllFromConfigFile(pathToConfigFile);
    }


};
#endif
