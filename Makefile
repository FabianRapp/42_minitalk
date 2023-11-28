CC=cc
#CFLAGS
NAME=minitalk

SERVER=server
PATH_SERVER=./server_code/
SOURCES_SERVER=server.c
SOURCES_SERVER := $(SOURCES_SERVER:%=$(PATH_SERVER)%)
OBJECTS_SERVER=$(SOURCES_SERVER:.c=.o)
LIB_SERVER=$(PATH_SERVER)lib_server.a

CLIENT=client
PATH_CLIENT=./client_code/
SOURCES_CLIENT=client.c
SOURCES_CLIENT := $(SOURCES_CLIENT:%=$(PATH_CLIENT)%)
OBJECTS_CLIENT := $(SOURCES_CLIENT:.c=.o)
LIB_CLIENT=$(PATH_CLIENT)lib_client.a


LIBFT_DIR=./libft
LIBFT=./libft/libft.a
LIBFT_PATH=$(LIBFT_DIR)/$(LIBFT)

all: $(NAME)

$(NAME): $(SERVER) $(CLIENT) clean

$(SERVER): $(LIBFT) $(OBJECTS_SERVER)
	ar rcs $(LIB_SERVER) $(OBJECTS_SERVER)
	$(CC) $(LIBFT) $(LIB_SERVER) -o $(SERVER)
	cc main.c $(LIBFT) -o test

$(CLIENT): $(LIBFT) $(OBJECTS_CLIENT)
	ar rcs $(LIB_CLIENT) $(OBJECTS_CLIENT)
	$(CC) $(LIBFT) $(LIB_CLIENT) -o $(CLIENT)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
#cp $(LIBFT_PATH) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS_SERVER) $(OBJECTS_CLIENT)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(SERVER) $(CLIENT) $(LIBFT) $(LIB_SERVER) $(LIB_CLIENT)
	rm -f a.out
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
