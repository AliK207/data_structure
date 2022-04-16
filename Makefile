TARGET= project
CFLAGS=-g -W -Wall -Wextra -MMD -O0 
LDFLAGS=-lm
DEP=$(SRC:.c=.d)
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

default: $(TARGET)

.o: .c
	gcc $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	gcc $(LDFLAGS) $^ -o $@


.PHONY: clean
clean:
	rm -f src/*.o
	rm -f $(TARGET)
