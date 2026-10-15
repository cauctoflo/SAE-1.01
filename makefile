CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude -MMD -MP
LDFLAGS =
NAME    = sae
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=build/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -fsanitize=address,undefined
debug: LDFLAGS += -fsanitize=address,undefined
debug: clean $(NAME)

clean:
	rm -rf build $(NAME)

-include $(OBJ:.o=.d)
.PHONY: all clean debug