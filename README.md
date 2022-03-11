# HDC_HTTPS_Server
Rick Curtis HTTP Server for Smoothstack Capstone
INSTALLATION INSTRUCTIONS
----------------------------------------------------------
Install all of libhttpserver's dependencies
navigate to src directory
run make
run the executable ./serverStart

TESTING INSTRUCTIONS
----------------------------------------------------------
navigate to src/testing
run "source ./setEnvVals.sh" to set some environment variables or the env tests will fail
run "make tests"
run the executable "tests" (currently, no output means everything passed)
run "source ./unsetEnvVals.sh" to unset all the environment variables from testing
