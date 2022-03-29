#ifndef SERVERSTART_HPP
#define SERVERSTART_HPP
/*
    holds definitions used by main
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

//global logger so everyone can see it, and logging threads
Logger logger("logfile.txt");
std::thread logThread;
std::thread statsThread;

/* 
 * Print a message if the program was invoked incorrectly. This behavior does
 * not get logged anywhere
 */
auto printUsage = []()
{
    std::cout << "Invalid invocation. Valid invocation is one of the "
              << "following: " << std::endl;
    std::cout << "./serverStart (to use environment variables)\n";
    std::cout << "./serverStart <config_filename> (to use a config file)" << std::endl;
};


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
    logger.postType1Message(messageText);
}

//signal handler when we need to ctrl-c
void signalHandler( int signum ) {
   logger.quitFlag = true;
   logger.postType0Message("Interrupt signal received, shutting down.");
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

#endif