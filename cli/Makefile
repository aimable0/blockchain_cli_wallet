CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CPPFLAGS = -I.. -I../blockchain/v0.3 -I../blockchain/v0.3/transaction -I../blockchain/v0.3/provided -I../crypto
RM = rm -f

LIBB = ../blockchain/v0.3/libhblk_blockchain.a
LIBC = ../crypto/libhblk_crypto.a

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
NAME = cli

.PHONY: all clean fclean re libhblk_blockchain.a libhblk_crypto.a

all: $(LIBB) $(LIBC) $(NAME)

$(LIBB):
	$(MAKE) -C ../blockchain/v0.3

$(LIBC):
	$(MAKE) -C ../crypto

$(NAME): $(OBJ) $(LIBB) $(LIBC)
	$(CC) $(CFLAGS) $(OBJ) -o $@ -L../blockchain/v0.3 -lhblk_blockchain -L../crypto -lhblk_crypto -lssl -lcrypto -pthread

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(MAKE) -C ../blockchain/v0.3 clean
	$(MAKE) -C ../crypto clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C ../blockchain/v0.3 fclean
	$(MAKE) -C ../crypto fclean

re: fclean all

