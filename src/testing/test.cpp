//File that just has tests for the serverStarter methods
#include <iostream>
#include <cassert>
#include "../ServerConfig.hpp"

int main(int argc, char** argv){
    ServerConfig ss;
    //populate from environment
    assert(ss.isValidNumber("0"));
    assert(!ss.isValidNumber("-1"));
    assert(!ss.isValidNumber("123a0"));

    //you have to source the setEnvVals script for this to work or it fails because your environment variables aren't set
    ss.populateFromEnv();
    assert(ss.maxConnectionsPerIP == 5);
    assert(ss.portNumber == 8000);
    //assert(ss.reasonableThreadMax == 10);
    assert(ss.useHTTPS == false);
    assert(ss.threadPoolSize == 20);

    //populate from file
    std::string fileName = "../serverconfig.cfg";
    assert(1 == ss.populateFromConfigFile(fileName));
    assert(ss.portNumber == 8080);
    assert(ss.threadPoolSize == 10);
    assert(ss.maxConnectionsPerIP == 4);
    //assert(ss.reasonableThreadMax == 80); Look at this later
    assert(ss.useHTTPS == true);

    //need tests for the logger down here
    
    return 0;
}

