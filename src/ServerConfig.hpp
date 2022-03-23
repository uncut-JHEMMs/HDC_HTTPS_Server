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

//used for finding reasonable thread limit
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
    unsigned int threadPoolSize;
    //prevent the client from attempting to start a server with a ridiculuous
    //number of threads
    unsigned int reasonableThreadMax;//this may be premature optimization, leave this alone for now
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
    unsigned int getReasonableThreadMax(){
        return REASONABLE_COEFFICIENT * std::thread::hardware_concurrency();
    }

    //check that the port number is valid, 1-65535
    bool isPortValid(unsigned int port){
        if (port> 0 && port < 65536) return true;
        return false;
    }

    //check that a given input is a valid number (a positive integer)
    bool isValidNumber(std::string input) const{
        for (char c : input){
            if (!std::isdigit(c)) return false;
        }
        return true;
    }

    //need an array of boolean key tags and an array of numerical key tags

    //need a method that processes a numerical key-value

    //need a method that processes a boolean key-value 

    //parse an input line by looking at the key and then the value, and 
    //assign a member of the serverStarter appropritately
    int parseConfigLine(std::string& line){
        //make a string stream
        std::istringstream streamLine(line);
        std::string key, value;
        //find the key and value. If bad format, return a bad format code
        if (std::getline(streamLine, key, '=')){
            //assign the members
            if (std::getline(streamLine,value)){
                    //port key
                    if (key == portKey){
                        if (!isValidNumber(value)) return 2; //bad value code
                        portNumber = stoi(value);
                        if (!isPortValid(portNumber)) return 2;
                    }
                    //boolean keys
                    else if (key == blockKey){ //it turns out we probably never want to use this
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") doesBlock = false;
                        else doesBlock = true;
                    }
                    else if (key == dsKey){
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") dualStack = false;
                        else dualStack = true;
                    }
                    else if (key == tlsKey){
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") useHTTPS = false;
                        else useHTTPS = true;
                    }
                    //numerical keys
                    else if (key == TPSizeKey){
                        if (!isValidNumber(value)) return 2;
                        threadPoolSize = stoi(value);
                        //if we don't have at least 4 cores, turn off threadpool
                        if (std::thread::hardware_concurrency() <= 4){
                            threadPoolSize = 1;
                        }
                    }
                    else if (key == timeoutKey){
                        if (!isValidNumber(value)) return 2;
                        connectionTimeout = stoi(value);
                    }
                    else if (key == maxConnectKey){
                        if (!isValidNumber(value)) return 2;
                        maxConnections = stoi(value);
                    }
                    else if (key == ipConsKey){
                        if (!isValidNumber(value)) return 2;
                        maxConnectionsPerIP = stoi(value);
                    }   
                    //I let linux or openssl take care of issues with key not found or invalid cert/key type
                    else if (key == tlsCert){
                        pathToCertFile = value;
                    }
                    else if (key == tlsCertKey){
                        pathToKeyFile = value;
                    }
                    //some unrecognized key, so just return a code. Can also use this to do comments.
                    else {
                        return 3;
                    }
            }
            else return 1;
        }
        else return 1; //1 indicates bad formatting of config line

        return 0;
    }
    //populate the data members from the 
    //int return to use syscalls?
    bool populateFromConfigFile(const std::string& inputFile){
        //open the config file (if we don't find it, return false)
        std::ifstream file;
        file.open(inputFile);
        if (!file.is_open()){
            return false;
        }
        //get all the lines
        std::string line;
	    int res;
        while (getline(file, line)){
            //look at the return values of each line to determine valid config?
		    res = parseConfigLine(line);
		    if (res != 0){
                if (res == 1){
                    std::cout<< "Bad formatting of config file with config line: " << line << std::endl;
                }
                else if (res == 2){
                    std::cout << "Config value isn't a valid value with line: " << line << std::endl;
                }
                else if (res == 3){
                    std::cout << "Unrecognized setting in config file with line: " << line << std::endl;
                }
                return false;
            }
        }
        return true;
    }


    //
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
        reasonableThreadMax = 0;
        maxConnectionsPerIP = 0;
        useHTTPS = false;

    }
    //parameterized constructor takes a string that reporesents the path to 
    //a configuration file
    ServerConfig(std::string pathToConfigFile){
        populateFromConfigFile(pathToConfigFile);
    }


};
#endif
