LDLIBS = -lm
OBJS = rt.o bmp.o image.o utils.o
BIN = rt

CPPFLAGS = -D_GNU_SOURCE
CFLAGS ?= -Wall -Wextra -pedantic --std=c99

all: $(BIN)

$(BIN): $(OBJS)

clean:
	$(RM) $(OBJS)

.PHONY: all clean
