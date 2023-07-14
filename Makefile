CC=g++
CFLAGS=-std=c++11

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=main

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
