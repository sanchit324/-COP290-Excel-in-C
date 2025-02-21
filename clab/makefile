CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g  # Added -g for debugging
LDFLAGS = -lm

SRCS = init.c display.c io.c process.c stack.c dependent.c  # Added missing source files
OBJS = $(SRCS:.c=.o)
HEADERS = init.h display.h io.h process.h stack.h dependent.h  # Added missing headers

TARGET = sheet

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Added a 'run' command for convenience
run: $(TARGET)
	./$(TARGET)