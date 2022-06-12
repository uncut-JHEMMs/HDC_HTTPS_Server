/*
 * Program entrypoint. Here is where we determine the configuration method
 * (config file or environment variables), parse them into the ServerStarter,
 * and then use the SS to actually start the webserver.
 */
#include "ServerStart.hpp"

int main(int argc, char **argv)
{
    //check the invocation
    if (argc > 2){
        printUsage();
        return 1;
    }

    /* If we have one arg, it's an input file. If there are no additional args,
     * then we check the environment variables
     */
    ServerConfig sc;
    std::string configMessage = getTimeStamp();
    if (argc == 2)
    {
        // parse the config from the provided config file
        configMessage += "Server building from config file";
        std::string fileName{argv[1]};
        try{
            sc = ServerConfig(fileName);
        }
        catch(const char* c){
            std::cout << c << std::endl;
            exit(2);
        }
    }
    else if (argc == 1)
    {
        // attempt to parse the config from the environment variables
        configMessage += "Server building from environment variables";
        
	try{
            sc.populateAllFromEnv();
        }
        catch(const char* c){
            std::cout << c << std::endl;
            exit(2);
        }
	
    }

    //register signal handler with process
    signal(SIGINT, signalHandler);
    
    //create the webserver from the config
    httpserver::create_webserver cw;
    buildServer(cw, sc);
    httpserver::webserver ws(cw);
    
    //start loggers, log successful startup
    logger.postType0Message(configMessage);
    logThread = std::thread(&Logger::accessThread,&logger);
    statsThread = std::thread(&Logger::statsThread, &logger);
    
    //log the run configuration
    std::string configString = sc.toString();
    logger.postType0Message(configString);
    
    //register resources
    hello_world_resource hwr;
    ws.register_resource("/hello", &hwr, true);
    digest_resource dr;
    ws.register_resource("/digest",&dr, true);
    image_resource ir;
    ws.register_resource("/image",&ir, true);
    latency_resource lr;
    ws.register_resource("/latency",&lr, true);
    docs_resource xmlr;
    ws.register_resource("/xml", &xmlr, true);
    Demo_PFBY_Resource dpfbyr;
    ws.register_resource("dpfby", &dpfbyr, true);
    Big_PFBY_Resource bpfbyr;
    ws.register_resource("bpfby", &bpfbyr, true);
    Demo_UIB_Resource duibr;
    ws.register_resource("duib", &duibr, true);
    Big_UIB_Resource buibr;
    ws.register_resource("buib", &buibr, true);
    
    
    //run the server
    ws.start(true);
    return 0;
}
