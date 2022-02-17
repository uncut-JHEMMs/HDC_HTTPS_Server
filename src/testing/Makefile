TARGET = tests
OBJS = test.o
CC = g++
CFLAGS = -Wall -Werror -std=c++17 -I/usr/local/include -I/usr/local/include/httpserver -I/usr/include/p11-kit-1 -L/usr/local/lib
DEBUGFLAGS = -g 
LIBFLAGS = -lhttpserver -lmicrohttpd 
DEPS = serverStartup.hpp

%.o: %.cpp $(DEPS)
	$(CC) $(DEBUGFLAGS) -c -o $@ $< $(CFLAGS)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAGS)

.PHONY: clean

clean:
	rm -r *.o $(TARGET) tests