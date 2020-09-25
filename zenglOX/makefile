#makefile for zengl language

CC = $(CROSS_CC)
AR = $(CROSS_AR)

ARFLAGS = rc
CFLAGS = -g3 -ggdb -O0 -std=c99 -Wall -Wextra -Wno-switch -Wno-unused-variable -Wno-unused-parameter -Wno-uninitialized -Wno-sign-compare -Wno-return-type -Wno-missing-braces -Wno-pointer-sign -Wno-implicit-fallthrough -ffreestanding -nostdlib
CDEFINES = -D ZL_LANG_EN_WITH_CH -D ZL_EXP_OS_IN_ZENGLOX
STATIC_CFLAGS = $(CFLAGS) -fvisibility=hidden

SRCS = ../linux/zengl_main.c ../linux/zengl_parser.c ../linux/zengl_symbol.c ../linux/zengl_locals.c ../linux/zengl_assemble.c ../linux/zengl_ld.c ../linux/zenglrun_main.c ../linux/zenglrun_func.c ../linux/zenglrun_hash_array.c ../linux/zenglApi.c ../linux/zenglApi_BltModFuns.c ../linux/zenglDebug.c
OBJS = zengl_main.o zengl_parser.o zengl_symbol.o zengl_locals.o zengl_assemble.o zengl_ld.o zenglrun_main.o zenglrun_func.o zenglrun_hash_array.o zenglApi.o zenglApi_BltModFuns.o zenglDebug.o

zengl: zengl.o libzengl.a
	@echo "building $@"
	$(CC) $(CFLAGS) -Wl,-emain -Wl,-dynamic-linker,ld.so -o zengl zengl.o libzengl.a -I ../../../standard/include -I ../../.. -L ../../../ -lcommon -L ../../output -lc

libzengl.a : $(OBJS) longjmp.o setjmp.o
	@echo "building $@"
	$(AR) $(ARFLAGS) libzengl.a $(OBJS) longjmp.o setjmp.o

$(OBJS): $(SRCS) ../linux/zengl_global.h ../linux/zengl_global_header.h ../linux/zengl_locals.h ../linux/zengl_exportfuns.h ../../../standard/include/stdlib.h ./setjmp.h
	@echo "building $@ ..."
	$(CC) $(CDEFINES) $(STATIC_CFLAGS)  -c $(SRCS) -I . -I ../../../standard/include -I ../../.. -I ../linux -L ../../../ -lcommon -L ../../output -lc

zengl.o: zengl.c ../linux/zengl_exportfuns.h
	@echo "building $@"
	$(CC) $(CFLAGS) -c zengl.c -I ../../../standard/include -I ../../.. -I ../linux 

%.o: %.s
	@echo "building $@"
	$(CROSS_AS) -o $@ $< $(CROSS_AS_FLAGS)

clean:
	rm -fv *.o
	rm -fv libzengl.a
	rm -fv zengl

all: zengl

