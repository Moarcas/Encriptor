CC = gcc
CFLAGS = -g -Wall -lssl -lcrypto
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BINDIR = bin
BIN = $(BINDIR)/main

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -r $(BINDIR)/* $(OBJ)/* 