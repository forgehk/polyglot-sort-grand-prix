CC      = gcc
CFLAGS  = -std=c99 -O2 -Wall -Wextra
TARGET  = sort_demo
OBJS    = fast_sort.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c fast_sort.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
