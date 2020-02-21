BINARY=nbody
SRC_DIR=src

CXXFLAGS=
LDFLAGS=-lglfw -lGL -lGLEW -lm
CC=gcc

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(SOURCES:%.c=%.o)

.PHONY: all release debug clean

all: release

release: CXXFLAGS+=-O2
release: $(BINARY)

debug: LDFLAGS+=-g
debug: CXXFLAGS+=-g -O0
debug: $(BINARY)

clean:
	rm -f $(OBJECTS)

$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CXXFLAGS) -c -o $@ $<