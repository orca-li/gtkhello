PROJNAME ?= client
CC = gcc
SRC = main.c
OUT = $(PROJNAME)
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS   = $(shell pkg-config --libs gtk4)

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(GTK_CFLAGS) -o $@ $^ $(GTK_LIBS)

clean:
	rm -f $(OUT)

