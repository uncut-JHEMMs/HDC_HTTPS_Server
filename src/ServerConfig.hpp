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
    unsigned int threadPoolSize;
    //prevent the client from attempting to start a server with a ridiculuous
    //number of threads
    unsigned int reasonableThreadMax;//this may be premature optimization, leave this alone for now
    unsigned int maxConnectionsPerIP;
    //need connection timeout
    //https
    bool useHTTPS;
    //whether or not to support ipv6
    //bool dual_stack;
    //whether or not the server should block this thread
    //bool blocks
    //https needs these
    //std::string pathToKeyFile;
    //std::string pathToCertFile;
    

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
            if (std::getline(streamLine,value)){
                if (!isValidNumber(value)) return 2; //bad value code
                else{//here we assign the members
                    if (key == "UTOPIA_PORT"){
                        portNumber = stoi(value);
                    }
                    else if (key == "UTOPIA_TPS"){
                        threadPoolSize = stoi(value);
                    }
                    else if (key == "UTOPIA_CONNECTIONS"){
                        maxConnectionsPerIP = stoi(value);
                    }
                    else if (key == "UTOPIA_TLS"){
                        if (value == "0") useHTTPS = false;
                        else useHTTPS = true;
                    }
                    //some unrecognized key, so just return a code. Can also use this to do comments.
                    else {
                        return 3;
                    }
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
        std::vector<std::string> args;
        std::string line;
        while (getline(file, line)){
            //args.push_back(line);
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
        valc = getenv("UTOPIA_TPS");
        vals = valc;
        if (isValidNumber(vals)){
            threadPoolSize = stoi(vals);
        }
        valc = getenv("UTOPIA_CONNECTIONS");
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
        return true;
    }

    //default constructor assigns blank values
    ServerConfig(){
        portNumber = 0;
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