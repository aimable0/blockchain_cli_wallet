CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
INCLUDES = -I.. -I../blockchain/v0.2 -I../crypto

LIBB = ../blockchain/v0.3/libhblk_blockchain.a
LIBC = ../crypto/libhblk_crypto.a

NAME = cli
SRC = cli.c
OBJ = $(SRC:.c=.o)

# Blockchain source files
BLOCKCHAIN_SRCS = \
	../blockchain/v0.3/block_create.c \
	../blockchain/v0.3/block_destroy.c \
	../blockchain/v0.3/block_hash.c \
	../blockchain/v0.3/block_is_valid.c \
	../blockchain/v0.3/block_mine.c \
	../blockchain/v0.3/blockchain_create.c \
	../blockchain/v0.3/blockchain_deserialize.c \
	../blockchain/v0.3/blockchain_destroy.c \
	../blockchain/v0.3/blockchain_serialize.c \
	../blockchain/v0.3/blockchain_difficulty.c \
	../blockchain/v0.3/hash_matches_difficulty.c \
	../blockchain/v0.3/provided/_blockchain_destroy.c \
	../blockchain/v0.3/provided/_blockchain_print.c \
	../blockchain/v0.3/provided/_endianness.c \
	../blockchain/v0.3/provided/_genesis.c \
	../llist.c

BLOCKCHAIN_OBJS = $(BLOCKCHAIN_SRCS:.c=.o)

# Crypto source files
CRYPTO_SRCS = \
	../crypto/sha256.c \
	../crypto/ec_create.c \
	../crypto/ec_to_pub.c \
	../crypto/ec_from_pub.c \
	../crypto/ec_save.c \
	../crypto/ec_load.c \
	../crypto/ec_sign.c \
	../crypto/ec_verify.c

CRYPTO_OBJS = $(CRYPTO_SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(LIBB): $(BLOCKCHAIN_OBJS)
	ar rcs $(LIBB) $(BLOCKCHAIN_OBJS)

$(LIBC): $(CRYPTO_OBJS)
	ar rcs $(LIBC) $(CRYPTO_OBJS)

$(NAME): $(OBJ) $(LIBB) $(LIBC)
	$(CC) $(CFLAGS) $(OBJ) $(LIBB) $(LIBC) -o $(NAME) -lssl -lcrypto

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(BLOCKCHAIN_OBJS) $(CRYPTO_OBJS)

fclean: clean
	rm -f $(NAME) $(LIBB) $(LIBC)

re: fclean all
