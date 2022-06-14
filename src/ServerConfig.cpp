/*
 * This file holds the definitions to types and functions related to starting
 * a libhttpserver with customized behavior
 */
#include "ServerConfig.hpp"

    /*A reasonable number of threads is a coefficient of the available number
     * of physical threads. Remember that other stuff will probably be running
     * on this computer too, we don't want to try to grab everything
     */
    unsigned int ServerConfig::getReasonableThreadMax(){
        return REASONABLE_COEFFICIENT * std::thread::hardware_concurrency();
    }
    
    //populate all members from config file
    void ServerConfig::populateAllFromConfigFile(std::string& fileName){
        try{
            ConfigFileParser fileParser{fileName};
            populatePortFromConfig(fileParser);
            populateTimeoutFromConfig(fileParser);
            populateTPSFromConfig(fileParser);
            populateMaxConnectFromConfig(fileParser);
            populateDSFromConfig(fileParser);
            populateIPConConnectionsFromConfig(fileParser);
            //https group
            populateHTTPSFromConfig(fileParser);
            populateTLSCertFromConfig(fileParser);
            populateTLSKeyFromConfig(fileParser);
        }
        catch(const char* c){
            //just propagate the error up
            throw(c);
        }
    }

    //from env
    void ServerConfig::populateAllFromEnv(){
        try{
            populatePortFromEnv();
            populateMaxConnectionsFromEnv();
            populateIPConConnectionsFromEnv();
            populateTPSFromEnv();
            populateTimeoutFromEnv();
            populateDSFromEnv();
            //HTTPS group
            populateHTTPSFromEnv();
            populateTLSCertFromEnv();
            populateTLSKeyFromEnv();
        }
        catch (const char* c){
            throw(c);
        }
    }

    //just have methods for populate each, then a master that calls all
    //port from config
    bool ServerConfig::populatePortFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{portKey};
            std::string sPort = fileParser.getValueFromKey(val);
            portNumber = fileParser.convertToUInt(sPort);
        }
        catch(const char* c){
            throw("Error assigning port from config file. Check key and value.");
        }
        if (!isPortValid(portNumber)){
            throw("Port Number is outside valid range. Must be below 65536");
        }
        return true;
    }

    //port from Env
    void ServerConfig::populatePortFromEnv(){
        char* valc = getenv(portKey);
        if (valc == nullptr){
            throw("Couldn't find the port key in environment variables");
        }
        std::string vals(valc); 
        if (isValidNumber(vals)){
            portNumber = stoi(vals);
        }
        if (!isPortValid(portNumber)){
            throw("Port Number is outside valid range. Must be below 65536");
        }
    }

    //max connections
    bool ServerConfig::populateMaxConnectFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{maxConnectKey};
            std::string sMC = fileParser.getValueFromKey(val);
            maxConnections = fileParser.convertToUInt(sMC);
        }
        catch(const char* c){
            throw("Error assigning max connections from config file. Check key and value.");
        }
        return true;
    }

    //max connect from Env
    void ServerConfig::populateMaxConnectionsFromEnv(){
        char* valc = getenv(maxConnectKey);
        if (valc == nullptr){
            throw("Couldn't find the max connect key in environment variables");
        }
        std::string vals(valc); 
        if (isValidNumber(vals)){
            maxConnections = stoi(vals);
        }
    }

    //ip con connections
    bool ServerConfig::populateIPConConnectionsFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{ipConsKey};
            std::string sIC = fileParser.getValueFromKey(val);
            maxConnectionsPerIP = fileParser.convertToUInt(sIC);
        }
        catch(const char* c){
            throw("Error assigning IP connections from config file. Check key and value.");
        }
        return true;
    }

    //ip con connections env
    void ServerConfig::populateIPConConnectionsFromEnv(){
        char* valc = getenv(ipConsKey);
        if (valc == nullptr){
            throw("Couldn't find the ip con key in environment variables");
        }
        std::string vals(valc); 
        if (isValidNumber(vals)){
            maxConnectionsPerIP = stoi(vals);
        }
    }

    //Threadpool size
    bool ServerConfig::populateTPSFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{TPSizeKey};
            std::string sTPS = fileParser.getValueFromKey(val);
            threadPoolSize = fileParser.convertToUInt(sTPS);
            //extra check for TPS
            if (threadPoolSize > getReasonableThreadMax()){
                std::cout << "Your threadpool size is significantly higher than your machine's thread count. This is not recommended." << std::endl;
            }
        }
        catch(const char* c){
            throw("Error assigning TPS from config file. Check key and value.");
        }
        return true;
    }

    //TPS from env
    void ServerConfig::populateTPSFromEnv(){
        char* valc = getenv(TPSizeKey);
        if (valc == nullptr){
            throw("Couldn't find the tps key in environment variables");
        }
        std::string vals(valc); 
        if (isValidNumber(vals)){
            threadPoolSize = stoi(vals);
        }
        if (threadPoolSize > getReasonableThreadMax()){
                std::cout << "Your threadpool size is significantly higher than your machine's thread count. This is not recommended." << std::endl;
        }
    }

    //timeout
    bool ServerConfig::populateTimeoutFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{timeoutKey};
            std::string sTPS = fileParser.getValueFromKey(val);
            //set the member
            connectionTimeout = fileParser.convertToUInt(sTPS);
        }
        catch(const char* c){
            throw("Error assigning timeout from config file. Check key and value.");
        }
        return true;
    }

    //Timeout from env
    void ServerConfig::populateTimeoutFromEnv(){
        char* valc = getenv(timeoutKey);
        if (valc == nullptr){
            throw("Couldn't find the timeout key in environment variables");
        }
        std::string vals(valc); 
        if (isValidNumber(vals)){
            connectionTimeout = stoi(vals);
        }
    }

    //use HTTPS
    bool ServerConfig::populateHTTPSFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{tlsKey};
            std::string sHTTPS = fileParser.getValueFromKey(val);
            //set the member
            useHTTPS = fileParser.convertToBool(sHTTPS);
        }
        catch(const char* c){
            throw("Error assigning TLS from config file. Check key and value.");
        }
        return true;
    }

    //HTTPS from env
    void ServerConfig::populateHTTPSFromEnv(){
        char* valc = getenv(tlsKey);
        std::string vals(valc); 
        if (isValidNumber(vals)){
            useHTTPS = convertToBool(vals);
        }
    }

    //HTTPS cert
    bool ServerConfig::populateTLSCertFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{tlsCert};
            //no conversion needed
            pathToCertFile = fileParser.getValueFromKey(val);
        }
        catch(const char* c){
            throw("Error assigning cert from config file. Check key and value.");
        }
        return true;
    }

    //cert from env
    bool ServerConfig::populateTLSCertFromEnv(){
        char* valc = getenv(tlsCert);
        std::string vals{valc};
        pathToCertFile = vals;
        return true;
    }
    
    //HTTPS key
    bool ServerConfig::populateTLSKeyFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{tlsCertKey};
            //no conversion needed
            pathToKeyFile = fileParser.getValueFromKey(val);
        }
        catch(const char* c){
            throw("Error assigning TLS Key from config file. Check key and value.");
        }
        return true;
    }

    //key from env
    bool ServerConfig::populateTLSKeyFromEnv(){
        char* valc = getenv(tlsCertKey);
        std::string vals{valc};
        pathToKeyFile = vals;
        return true;
    }
    //Dual Stack
    bool ServerConfig::populateDSFromConfig(ConfigFileParser& fileParser){
        try{
            std::string val{dsKey};
            std::string sDS = fileParser.getValueFromKey(val);
            dualStack = fileParser.convertToBool(sDS);
        }
        catch(const char* c){
            throw("Error assigning dual stack from config file. Check key and value.");
        }
        return true;
    }

    //DS from env
    void ServerConfig::populateDSFromEnv(){
        char* valc = getenv(dsKey);
        std::string vals(valc); 
        if (isValidNumber(vals)){
            dualStack = convertToBool(vals);
        }
    }

    /*
        returns a string representation of the current config
    */
    std::string ServerConfig::toString(){
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
    ServerConfig::ServerConfig(){
        portNumber = 8080;
        doesBlock = true;
        dualStack = true;
        connectionTimeout = 5;
        maxConnections = 10;
        threadPoolSize = 20;
        //reasonableThreadMax = 0;
        maxConnectionsPerIP = 11;
        useHTTPS = true;
	pathToKeyFile = std::string{"../PKI/ca/mykey.pem"};
	pathToCertFile = std::string{"../PKI/certs/mycert.crt"};

    }
    //parameterized constructor takes a string that reporesents the path to 
    //a configuration file
    ServerConfig::ServerConfig(std::string pathToConfigFile){
        populateAllFromConfigFile(pathToConfigFile);
    }
    //helper
    bool ServerConfig::convertToBool(const std::string& value){
        if (!isValidNumber(value)){
                throw("Value in config file isn't a number");
            }
            else{
                if (value == "0"){
                    return false;
                }
                else{
                    return true;
                }
            }
    }
    //TODO: move these to parser only
    bool ServerConfig::isValidNumber(std::string input) const{
        for (const char& c : input){
            if (!std::isdigit(c)) return false;
        }
        return true;
        }
    //check that the port number is valid, 1-65535
    bool ServerConfig::isPortValid(unsigned int port){
        if (port> 0 && port < 65536) return true;
        return false;
    }
