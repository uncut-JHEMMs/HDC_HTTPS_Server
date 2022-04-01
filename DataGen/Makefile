CC=g++ #special makefile constant
CFLAGS=-I. -g -std=c++17 -Wall -Werror #include this directory when searching for includes, special makefile constant
DEPS = #empty for now
OBJ = SimplifyNames.o #all the intermediate object files go here

%.o: %.cpp $(DEPS) #build every o from it's respective cpp, and check for updates in any of DEPS
	$(CC) -c -o $@ $^ $(CFLAGS)
#run the compile command with -c, $@ is the '%.o', and $< is the '%.cpp' 

#target rules requires OBJ to be built
target: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean # don't mess with files named "clean", just use the clean rule

clean: #remove all the generated object files and the executable
	rm -f *.o target 
