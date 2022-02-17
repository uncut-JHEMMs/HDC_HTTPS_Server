TARGET = serverStart
OBJS = main.o
CC = g++
CFLAGS = -Wall -Werror -std=c++17 -I/usr/local/include -I/usr/local/include/httpserver -I/usr/include/p11-kit-1 -L/usr/local/lib
DEBUGFLAGS = -ggdb 
LIBFLAGS = -lhttpserver -lmicrohttpd 
DEPS = serverStartup.hpp

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS)
debug : $(OBJS)
	g++ -o debug $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS) main.o

tests : test.o
	g++ -o $@ $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS) test.o

.PHONY: clean

clean:
	rm -r *.o $(TARGET) $(DEBUGTARGET) tests