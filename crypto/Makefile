CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu90 -ggdb3
CPPFLAGS = -I.
OBJDIR = targets/
RM = rm -f

SRC = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ = $(addprefix $(OBJDIR), $(SRC:%.c=%.o))
NAME = libhblk_crypto.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): hblk_crypto.h $(OBJDIR) $(OBJ)
	ar rcs $@ $(OBJ)

$(OBJDIR)%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJDIR)

re: fclean all
