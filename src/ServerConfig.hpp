#ifndef SERVERSTARTER_HPP
#define SERVERSTARTEr_HPP
/*
 * This file holds the definitions to types and functions related to starting
 * a libhttpserver with customized behavior
 */
#include <fstream>
#include <thread> //hardware concurrency
#include <regex> //isNumber
#include <string>
#include <cstdlib>
#include <sstream> //parse line
#include <vector>
#include <httpserver.hpp>

//used for finding reasonable thread limit
#define REASONABLE_COEFFICIENT 4
//environment variable names
//put all of these in a container so we can just iterate over all of them
#define portKey "UTOPIA_PORT"
#define TPSizeKey "UTOPIA_TPS"
#define maxConnectKey "UTOPIA_CONNECTIONS"
#define tlsKey "UTOPIA_TLS"

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
    //log file
    //std::ostream log?
    

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
                    if (key == "UTOPIA_PORT"){
                        if (!isValidNumber(value)) return 2; //bad value code
                        portNumber = stoi(value);
                    }
                    else if (key == "UTOPIA_TIMEOUT"){
                        if (!isValidNumber(value)) return 2;
                        connectionTimeout = stoi(value);
                    }
                    else if (key == "UTOPIA_BLOCK"){ //it turns out we probably never want to use this
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") doesBlock = false;
                        else doesBlock = true;
                    }
                    else if (key == "UTOPIA_DUAL_STACK"){
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") dualStack = false;
                        else dualStack = true;
                    }
                    else if (key == "UTOPIA_TPS"){
                        if (!isValidNumber(value)) return 2;
                        threadPoolSize = stoi(value);
                        //if we don't have at least 4 cores, turn off threadpool
                        if (std::thread::hardware_concurrency() <= 4){
                            threadPoolSize = 1;
                        }
                    }
                    else if (key == "UTOPIA_CONNECTIONS"){
                        if (!isValidNumber(value)) return 2;
                        maxConnections = stoi(value);
                    }
                    else if (key == "UTOPIA_IP_CON_CONNECTIONS"){
                        if (!isValidNumber(value)) return 2;
                        maxConnectionsPerIP = stoi(value);
                    }   
                    else if (key == "UTOPIA_TLS"){
                        if (!isValidNumber(value)) return 2;
                        if (value == "0") useHTTPS = false;
                        else useHTTPS = true;
                    }
                    else if (key == "UTOPIA_TLS_CERT"){
                        pathToCertFile = value;
                    }
                    else if (key == "UTOPIA_TLS_KEY"){
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
        while (getline(file, line)){
            //look at the return values of each line to determine valid config?
            parseConfigLine(line);
        }
        return true;
    }

    //populate the data members from environment variables. Use the names defined above in this file.
    bool populateFromEnv(){
        //get each of the environment variables in the list
        char* valc = getenv("UTOPIA_PORT");
        std::string vals(valc); 
        if (isValidNumber(vals)){
            portNumber = stoi(vals);
        }
        valc = getenv("UTOPIA_TIMEOUT");
        vals = valc;
        if (isValidNumber(vals)){
            connectionTimeout = stoi(vals);
        }
        valc = getenv("UTOPIA_BLOCK");
        vals = valc;
        if (isValidNumber(vals)){
            if (vals == "0") doesBlock = false;
            else doesBlock = true;
        }
        valc = getenv("UTOPIA_DUAL_STACK");
        vals = valc;
        if (isValidNumber(vals)){
            if (vals == "0") dualStack = false;
            else dualStack = true;
        }
        valc = getenv("UTOPIA_TPS");
        vals = valc;
        if (isValidNumber(vals)){
            threadPoolSize = stoi(vals);
        }
        valc = getenv("UTOPIA_CONNECTIONS");
        vals = valc; 
        if (isValidNumber(vals)){
            maxConnections = stoi(vals);
        }
        valc = getenv("UTOPIA_IP_CON_CONNECTIONS");
        vals = valc; 
        if (isValidNumber(vals)){
            maxConnectionsPerIP = stoi(vals);
        }
        valc = getenv("UTOPIA_TLS");
        vals = valc; 
        if (isValidNumber(vals)){
            if (vals == "0") useHTTPS = false;
            else useHTTPS = true;
        }
        //strings at the bottom here
        valc = getenv("UTOPIA_TLS_CERT");
        vals = valc;
        pathToCertFile = vals;

        valc = getenv("UTOPIA_TLS_KEY");
        vals = valc;
        pathToKeyFile = vals; 
        return true;
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