TARGET=prog
CC=gcc
CFLAGS=-Wall -g
LIBS= -lncurses

OBJ = $(patsubst %.c, %.o, $(wildcard src/*.c) )
SRC = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

.PHONY: clean
.DEFAULT: $(TARGET)

$(TARGET): $(OBJ) $(HEADERS)
	$(CC) -o $@ $(OBJ) $(LIBS)
	rm -f src/*.o 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)