CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CPPFLAGS = -I. -Itransaction -Iprovided -I../../ -I../../crypto
AR = ar
ARFLAGS = rcs
RM = rm -f

NAME = libhblk_blockchain.a

SRC = \
       blockchain_create.c \
       blockchain_deserialize.c \
       blockchain_destroy.c \
       blockchain_difficulty.c \
       blockchain_serialize.c \
       block_create.c \
       block_destroy.c \
       block_hash.c \
       block_is_valid.c \
       block_mine.c \
       hash_matches_difficulty.c \
       transaction/coinbase_create.c \
       transaction/coinbase_is_valid.c \
       transaction/transaction_create.c \
       transaction/transaction_destroy.c \
       transaction/transaction_hash.c \
       transaction/transaction_is_valid.c \
       transaction/tx_in_create.c \
       transaction/tx_in_sign.c \
       transaction/tx_out_create.c \
       transaction/unspent_tx_out_create.c \
       transaction/update_unspent.c \
       provided/_blockchain_destroy.c \
       provided/_blockchain_print.c \
       provided/_endianness.c \
       provided/_genesis.c \
       provided/_print_hex_buffer.c \
       provided/_transaction_print_brief.c \
       provided/_transaction_print.c \
       ../../llist.c

OBJ = $(SRC:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c blockchain.h transaction/transaction.h ../../llist.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
