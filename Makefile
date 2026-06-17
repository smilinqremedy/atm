CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=gnu99 -Isrc
SRC = src/main.c src/auth.c src/system.c
OBJ = $(SRC:.c=.o)
TARGET = atm_system

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p data
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) data/*.tmp

fclean: clean
	rm -rf data/*.txt

.PHONY: all clean fclean