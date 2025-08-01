CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

SRC     = main.c
OUT     = vbc

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

fclean: clean

re: fclean all

.PHONY: all clean fclean re