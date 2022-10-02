IDIR =../include
CC=gcc
CFLAGS = -I. -std=c99

CFLAGS += -Wall -Wextra -Werror -Wno-unused-parameter

ODIR=obj
LDIR =../lib

SOURCE_DIR=.

HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)

#_OBJ = chunk.o debug.o main.o memory.o value.o
OBJ = $(patsubst %,$(ODIR)/%,$(SOURCES:.c=.o))

obj/%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

chunk: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~~
