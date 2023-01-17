CC = gcc
CFLAGS = -g -Wall -lssl -lcrypto
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BINDIR = bin
BIN = $(BINDIR)/main

all:$(BIN)
	# ./bin/main date.txt
	# ./bin/main date.txt permutari.txt

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r $(BINDIR)/* $(OBJ)/* 