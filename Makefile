LDLIBS = -lm
OBJS = rt.o src/bmp.o src/image.o src/camera.o src/utils/pvect.o src/utils/alloc.o
BIN = rt

CPPFLAGS = -D_GNU_SOURCE -iquote includes/
CFLAGS ?= -Wall -Wextra -pedantic --std=c99

all: $(BIN)

$(BIN): $(OBJS)

clean:
	$(RM) $(OBJS)

.PHONY: all clean
