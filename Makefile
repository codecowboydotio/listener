# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = listener

# the install directory
PREFIX = /usr/local/listener
DESTDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

list: 
	ls -la;

install:
	install -d $(DESTDIR)
	install $(TARGET) $(DESTDIR)

uninstall: 
	rm -rf $(PREFIX)

clean:
	$(RM) $(TARGET)
