/*
 * Program entrypoint. Here is where we determine the configuration method
 * (config file or environment variables), parse them into the ServerStarter,
 * and then use the SS to actually start the webserver.
 */
#include <iostream>
#include <httpserver.hpp>
#include "ServerConfig.hpp"
#include "Endpoints.hpp"
#include "Logger.hpp"
#include <fstream>
#include <cassert>
#include <csignal>
#include <ctime>

/* 
 * Print a message if the program was invoked incorrectly. This behavior does
 * not get logged anywhere
 */
void printUsage()
{
    std::cout << "Invalid invocation. Valid invocation is one of the "
              << "following: " << std::endl;
    std::cout << "./serverStart (to use environment variables)\n";
    std::cout << "./serverStart <config_filename> (to use a config file)" << std::endl;
}

//global logger so everyone can see it
Logger logger("logfile.txt");
std::thread logThread;
std::thread statsThread;


//get the current time for use in logs
std::string getTimeStamp(){
    time_t now = time(0);
    char* dt = ctime(&now);
    //strip the newline
    std::string message(dt);
    return message;
}

//also define the access logger function that gets passed to the server's log_access method
void custom_access_log(const std::string& url) {
    //get the current time stamp first, then print the url and method
    std::string messageText = getTimeStamp();
    messageText += "Access logged at ";
    messageText += url;
    LogMessage message;
    message.setMessageType(LogMessageType::REQUESTLOG);
    message.setResponseCode(LogResponseCode::OK);
    message.setTextMessage(messageText);
    logger.postMessage(message);
}

//signal handler when we need to ctrl-c
void signalHandler( int signum ) {
   logger.quitFlag = true;
   std::string quitMessage("Interrupt signal received, shutting down.");
   LogMessage message;
   message.setMessageType(LogMessageType::NONACCESS);
   message.setResponseSize(0);
   message.setResponseCode(LogResponseCode::OK);
   message.setTextMessage(quitMessage);
   logger.postMessage(message);
   // cleanup and close up stuff here  
   statsThread.join();
   logThread.join();
   exit(signum);  
} 

//build out the webserver by looking at each member of the serverStarter and
//making an appropriate call to httpserver's member
int buildServer(httpserver::create_webserver& cw, const ServerConfig& sc){
    //port number is required
    cw = httpserver::create_webserver(sc.portNumber);
    //Max concurrent Connections
    cw.max_connections(sc.maxConnections);
    //max connections per IP
    cw.per_IP_connection_limit(sc.maxConnectionsPerIP);
    //thread pool size, start method of internal select is the default, so it isn't totally necessary to specify that
    cw.max_threads(sc.threadPoolSize);
    //https stuff, it's either all or none
    if (sc.useHTTPS){
        cw.use_ssl();
        cw.https_mem_key(sc.pathToKeyFile);
        cw.https_mem_cert(sc.pathToCertFile);
    }
    else {
        cw.no_ssl();
    }
    //timeout
    cw.connection_timeout(sc.connectionTimeout);
    cw.log_access(custom_access_log);
    //digest authentication is implicitly true, just need to have digest endpoints
    // dual stack/ipv6
    if (sc.dualStack) cw.use_dual_stack();
    else cw.no_dual_stack();
    // blocking is handled in main as the argument to webserver::start
    return 0;
}

int main(int argc, char **argv)
{
    /* If we have one arg, it's an input file. If there are no additional args,
     * then we check the environment variables
     */
    ServerConfig sc;
    std::string configMessage = getTimeStamp();
    bool res;
    if (argc == 2)
    {
        // parse the config file, do the easy known good one to make sure server runs
        configMessage += "Server building from config file";
        res = sc.populateFromConfigFile(argv[1]);
        if (!res) exit(1);
        
    }
    else if (argc == 1)
    {
        // get the environment variables
        configMessage += "Server building from environment variables";
        res = sc.populateFromEnv();
        if (!res) exit(1);
    }
    else
    {
        printUsage();
        return 1;
    }
    //start logging messages
    LogMessage message;
    message.setMessageType(LogMessageType::NONACCESS);
    message.setResponseCode(LogResponseCode::OK);
    message.setTextMessage(configMessage);
    logger.postMessage(message);
    //register signal handler with process
    signal(SIGINT, signalHandler); 
    //start log thread
    logThread = std::thread(&Logger::accessThread,&logger);
    statsThread = std::thread(&Logger::statsThread, &logger);
    // here we use the starter to build the webserver_create
    httpserver::create_webserver cw;

    //the config is valid, so build the server
    buildServer(cw, sc);
    httpserver::webserver ws(cw);
    //dump the server config to the log
    std::string configString = sc.toString();
    message.setMessageType(LogMessageType::NONACCESS);
    message.setResponseCode(LogResponseCode::OK);
    message.setTextMessage(configString);
    logger.postMessage(message);
    
    //register resources
    //TODO break this back out
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr, true);
    digest_resource dr;
    ws.register_resource("/digest",&dr, true);
    image_resource ir;
    ws.register_resource("/image",&ir, true);
    latency_resource lr;
    ws.register_resource("/latency",&lr, true);
    ws.start(true);

    return 0;
}
