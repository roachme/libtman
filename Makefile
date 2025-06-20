LIB_STATIC=libtman.a
LIB_DYNAMIC=libtman.so
LIB_CC=gcc
LIB_SRCS=$(wildcard src/*.c)
LIB_OBJS=$(patsubst %.c, %.o, $(LIB_SRCS))
LIB_DEPS=$(wildcard src/*.h)
# TODO: Add option -Wextra
LIB_CFLAGS=-I src -Wall -g3 -fbounds-check

.PHONY: lib_static lib_dynamic lib_clean lib_debug lib_release


%.o: %.c $(LIB_DEPS)
	$(LIB_CC) -c -o $@ $< $(LIB_CFLAGS)

lib_static: $(LIB_OBJS)
	ar rcs $(LIB_STATIC) $^

# TODO: add dynamic library

lib_clean:
	rm -rf $(LIB_OBJS) $(LIB_STATIC) $(LIB_DYNAMIC)

