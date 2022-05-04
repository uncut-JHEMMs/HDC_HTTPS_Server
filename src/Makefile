TARGET = serverStart
DEBUGTARGET = debug
OBJS = ServerConfig.o Logger.o Parser.o main.o tinyxml2.o Queries.o
CC=g++
CFLAGS=-Wall -Werror -std=c++17 `pkg-config --cflags --libs libhttpserver` -I.
#The above HAS to be the tildes, the $() expansion for pkg-config command doesn't work
DEBUGFLAGS = -g
LIBFLAGS = -pthread
DEPS =  Parser.hpp Logger.hpp ServerStart.hpp ServerStats.hpp tinyxml2.h Queries.hpp

#the $@ means "the thing on the left of the rule", while $^ is "the thing on the right of the rule"
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(DEBUGFLAGS)

$(TARGET) : $(OBJS) #$(DEPS) deps is already in the little files
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS)

$(DEBUGTARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS)

tests : test.o
	$(CC) -o $@ $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS)

.PHONY: clean

clean:
	rm -rf *.o $(TARGET) $(DEBUGTARGET)
