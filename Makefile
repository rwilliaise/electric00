NAME=electric00
CC=gcc
CFLAGS=$(shell pkg-config --cflags raylib) -Wall -g
LDFLAGS=$(shell pkg-config --libs raylib) -lm
OBJECTS=electric00.o

$(NAME): $(OBJECTS)
