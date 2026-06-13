CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
INCLUDES = -I.. -I../blockchain/v0.2 -I../crypto

LIBB = ../blockchain/v0.3/libhblk_blockchain.a
LIBC = ../crypto/libhblk_crypto.a

SRC = cli.c
OBJ = $(SRC:.c=.o)
NAME = cli

.PHONY: all clean fclean re

all: $(NAME)

$(LIBB): ../blockchain/v0.3/stub_blockchain.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o /tmp/stub_blockchain.o ../blockchain/v0.3/stub_blockchain.c
	@mkdir -p $(dir $(LIBB))
	ar rcs $(LIBB) /tmp/stub_blockchain.o

$(LIBC): ../crypto/stub_crypto.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o /tmp/stub_crypto.o ../crypto/stub_crypto.c
	@mkdir -p $(dir $(LIBC))
	ar rcs $(LIBC) /tmp/stub_crypto.o

$(NAME): $(OBJ) $(LIBB) $(LIBC)
	$(CC) $(CFLAGS) $(OBJ) $(LIBB) $(LIBC) -o $(NAME) -lssl -lcrypto

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) /tmp/stub_blockchain.o /tmp/stub_crypto.o

fclean: clean
	rm -f $(NAME) $(LIBB) $(LIBC)

re: fclean all