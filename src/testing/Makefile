#makefiles for these little generation and testing programs, more than one in here

CC=g++ #special makefile constant
CFLAGS=-I. -g -std=c++17 -Wall #-Werror #include this directory when searching for includes, special makefile constant
DEPS = UserRecord.hpp
TARGET = SimplifyNames
XMLSTUFF = tinyxml2.h #`pkg-config libxml++-2.6 --cflags --libs`
OBJ = SimplifyNames.o #all the intermediate object files go here


%.o: %.cpp $(DEPS) #build every o from it's respective cpp, and check for updates in any of DEPS
	$(CC) -c -o $@ $^ $(CFLAGS)
#run the compile command with -c, $@ is the '%.o', and $< is the '%.cpp' 

#target rules requires OBJ to be built
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

#rules for the simple xml program
lilXML: 
	$(CC) -o $@ $^ $(CFLAGS) tinyxml2.cpp SimpleXMLOutput.cpp

testDataManip.exe: dataManipulatorTests.cpp
	$(CC) -o testDataManip.exe $(CFLAGS) ../DataManipulator.cpp dataManipulatorTests.cpp 


.PHONY: clean # don't mess with files named "clean", just use the clean rule

clean: #remove all the generated object files, the executable and the generated files
	rm -rf *.o $(TARGET) lilXML assignFromTransactions *.exe *.cdf
