TARGET = serverStart
OBJS = main.o
CC = clang++
CFLAGS = -Wall -Werror -std=c++17 -I/usr/local/include -I/usr/local/include/httpserver -I/usr/include/p11-kit-1 -L/usr/local/lib
DEBUGFLAGS = -g 
LIBFLAGS = -lhttpserver -lmicrohttpd -pthread
DEPS = serverStartup.hpp

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS)
debug : $(OBJS)
	$(CC) -o debug $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS) main.o

tests : test.o
	$(CC) -o $@ $(CFLAGS) $(LIBFLAGS) $(DEBUGFLAGS) test.o

explicit:
	clang++ -o serverStart -g -Wall -Werror -std=c++17 -I/usr/local/include -I/usr/local/include/httpserver -I/usr/include/p11-kit-1 -L/usr/local/lib -lhttpserver -lmicrohttpd -pthread main.cpp
.PHONY: clean

clean:
	rm -r *.o $(TARGET) $(DEBUGTARGET) tests
