CC = gcc
CFLAGS = -g -Wall
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BINDIR = bin
BIN = $(BINDIR)/main

all:$(BIN)
	./bin/main date.txt 

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r $(BINDIR)/* $(OBJ)/* 