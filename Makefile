CC=cc
#CFLAGS
NAME1=client
NAME2=server
SERVER=server.c
CLIENT=client.c
S_NAME=server
C_NAME=client
SOURCES= 
OBJECTS = $(SOURCES:.c=.o)
LIBFT_DIR = ./libft
LIBFT = libft.a
LIBFT_PATH = $(LIBFT_DIR)/$(LIBFT)


all: $(NAME) clean

$(NAME):libft $(OBJECTS) $(CLIENT) $(SERVER)

$(SERVER):
	$(CC) $(OBJECTS) $(LIBFT) $(SERVER) -o $(S_NAME)

$(CLIENT): libft $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBFT) $(CLIENT) -o $(C_NAME)

$(LIBFT_PATH):
	$(MAKE) -C $(LIBFT_DIR)
	cp $(LIBFT_PATH) $(LIBFT)

libft: $(LIBFT_PATH)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:

	rm -f $(OBJECTS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME1) $(NAME2) $(LIBFT)
	rm -f a.out
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
