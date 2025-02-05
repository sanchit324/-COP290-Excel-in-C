CC = gcc
CFLAGS = -Wall -Wextra -I./src

SRC = src/sheet.c \
      src/cell/cell.c \
      src/parser/parser.c \
      src/graph/graph.c \
      src/viewport/viewport.c \
      src/error/error.c \
      src/util/util.c

OBJ = $(SRC:.c=.o)

sheet: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) sheet