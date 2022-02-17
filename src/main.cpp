/*
 * Program entrypoint. Here is where we determine the configuration method
 * (config file or environment variables), parse them into the ServerStarter,
 * and then use the SS to actually start the webserver.
 */
#include <iostream>
#include <httpserver.hpp>
#include "ServerConfig.hpp"
#include <fstream>
#include <cassert>

//use this for testing
class hello_world_resource : public httpserver::http_resource {
 public:
     const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&);
     void set_some_data(const std::string &s) {data = s;}
     std::string data;
};

// Using the render method you are able to catch each type of request you receive
const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req) {
    // It is possible to store data inside the resource object that can be altered through the requests
    std::cout << "Data was: " << data << std::endl;
    std::string datapar = req.get_arg("data");
    set_some_data(datapar == "" ? "no data passed!!!" : datapar);
    std::cout << "Now data is:" << data << std::endl;

    // It is possible to send a response initializing an http_string_response that reads the content to send in response from a string.
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello World!!!", 200));
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
    //threadpoolsize
    cw.max_connections(sc.threadPoolSize);
    //max connections
    cw.per_IP_connection_limit(sc.maxConnectionsPerIP);
    //https stuff later
    if (sc.useHTTPS){
        cw.use_ssl();
        cw.https_mem_key(sc.pathToKeyFile);
        cw.https_mem_cert(sc.pathToCertFile);
    }
    else {
        cw.no_ssl();
    }
    //need to add connection timeout
    //need to add dual stack/ipv6
    return 0;
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
    // here we use the starter to build the webserver_create
    httpserver::create_webserver cw;
    buildServer(cw, sc);
    httpserver::webserver ws(cw);
      //forgot to add the resources down here
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr, true);
    // here we run the server, just have it block for now
    ws.start(true);
  

    return 0;
}