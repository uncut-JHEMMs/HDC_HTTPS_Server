TARGET = serverStart
DEBUGTARGET = debug
OBJS = ServerConfig.o Logger.o Parser.o main.o tinyxml2.o Queries.o Marts.o
CC=g++
CFLAGS=-Wall -Werror -std=c++17 `pkg-config --cflags --libs libhttpserver` `xml2-config --cflags --libs`
DEBUGFLAGS = -g
LIBFLAGS = -pthread
DEPS =  Parser.hpp Logger.hpp ServerStart.hpp ServerStats.hpp tinyxml2.h Queries.hpp Marts.hpp Endpoints.hpp tree.h
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(DEBUGFLAGS)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^   $(CFLAGS) $(LIBFLAGS)

$(DEBUGTARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS)

.PHONY: clean

clean:
	rm -rf *.o $(TARGET) $(DEBUGTARGET)
