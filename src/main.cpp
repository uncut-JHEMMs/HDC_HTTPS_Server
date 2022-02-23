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

//global logger so everyone can see it
Logger logger("logfile.txt");

//forward declaration
void custom_access_log(const std::string& url);
//signal handler to allow logfile to close properly
void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   // cleanup and close up stuff here  
   logger.quit();
   // join the logging thread(later)  
   //loggingThread.join();
   exit(signum);  
} 

/* Print a message if the program was invoked incorrectly. This behavior does
 * not get logged anywhere
 */
void printUsage()
{
    std::cout << "Invalid invocation. Valid invocation is one of the "
              << "following: " << std::endl;
    std::cout << "./serverStart (to use environment variables)\n";
    std::cout << "./serverStart <config_filename> (to use a config file" << std::endl;
}


//build out the webserver by looking at each member of the serverStarter and
//making an appropriate call to httpserver's member
int buildServer(httpserver::create_webserver& cw, ServerConfig& sc){
    //port number is required
    cw = httpserver::create_webserver(sc.portNumber);
    /*
       digest authentication is required, but doc says it's implied. There is
       no change in behavior if we add it, so we can just let it be implied and
       build all the endpoints as digest resources.
    
    cw.digest_auth();
    */
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
    //logfile test
    cw.log_access(custom_access_log);

    //digest authentication is implicitly true, just need to have digest endpoints
    // dual stack/ipv6
    if (sc.dualStack) cw.use_dual_stack();
    else cw.no_dual_stack();
    //hardcode use ipv6 here for testing
    //cw.use_ipv6();
    // blocking is handled in main as the argument to webserver::start
    return 0;
}


//also define the access logger function that gets passed to the server's log_access method
void custom_access_log(const std::string& url) {
    std::string message = "Access logged";
    logger.postMessage(message);
}

int main(int argc, char **argv)
{
    /* If we have one arg, it's an input file. If there are no additional args,
     * then we check the environment variables
     */
    ServerConfig sc;
    if (argc == 2)
    {
        // parse the config file, do the easy known good one to make sure server runs
        std::cout << "Parse from config file" << std::endl;
        sc.populateFromConfigFile(argv[1]);
    }
    else if (argc == 1)
    {
        // get the environment variables
        std::cout << "Check the environment variables" << std::endl;
        sc.populateFromEnv();
    }
    else
    {
        printUsage();
        return 1;
    }
    //register signal handler with process
    signal(SIGINT, signalHandler); 
    // here we use the starter to build the webserver_create
    httpserver::create_webserver cw;
    buildServer(cw, sc);
    httpserver::webserver ws(cw);
    //forgot to add the resources down here
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr, true);

    //digest part, can we mix digest and non-digest?
    digest_resource dr;
    ws.register_resource("/digest",&dr, true);
    // here we run the server, just have it block for now. Having it not block makes it seem like it doesn't run
    ws.start(sc.doesBlock);
    return 0;
}
